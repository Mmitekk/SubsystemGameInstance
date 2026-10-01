# Auto-load Blueprint subsystems at editor startup (optional)

## Why

The engine generates dedicated getter nodes (`Get TimeSubsystem`, category **GameInstance Subsystems**)
only for **loaded** subsystem classes (`UK2Node_GetSubsystem` uses `GetDerivedClasses`).
A subsystem Blueprint you never opened in this editor session has no node yet.

To make nodes appear for **all** current and future subsystems on every editor start —
without opening each Blueprint by hand — add the deferred loader below to your **game module**.
It runs **in the editor only** (`WITH_EDITOR`), so packaged builds are unaffected.
Loading the class is enough: GameInstance subsystems *are* auto-instanced by the engine
via `GetDerivedClasses` at GameInstance init (unlike Editor subsystems, no activation call needed).

## 1. Module header (`Source/YourProject/Public/YourProject.h`)

Replace the file content (rename `YourProject` / `FYourProjectGameModule` to your names):

```cpp
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FYourProjectGameModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
#if WITH_EDITOR
	// Deferred loading: runs after the editor finished initializing.
	void LoadBlueprintSubsystems();
	// Runs once the Asset Registry finished initial discovery, then loads subsystems.
	void LoadBlueprintSubsystemsNow();

	FDelegateHandle PostEngineInitHandle;
	FDelegateHandle FilesLoadedHandle;
#endif
};
```

## 2. Module source (`Source/YourProject/Private/YourProject.cpp`)

Replace the file content (rename `YourProject` / `FYourProjectGameModule` to your names):

```cpp
// Fill out your copyright notice in the Description page of Project Settings.

#include "YourProject.h"

#if WITH_EDITOR
#include "Misc/CoreDelegates.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Engine/Blueprint.h"
#include "Blueprint/BlueprintSupport.h"
#include "SubsystemGameInstance.h"
#endif

void FYourProjectGameModule::StartupModule()
{
	Super::StartupModule();

#if WITH_EDITOR
	// Dedicated "Get ..." nodes (UK2Node_GetSubsystem) are generated only for LOADED
	// subsystem classes, and the Asset Registry is still scanning at module startup,
	// so loading is deferred until the editor finished initializing.
	PostEngineInitHandle = FCoreDelegates::OnPostEngineInit.AddRaw(this, &FYourProjectGameModule::LoadBlueprintSubsystems);
#endif
}

void FYourProjectGameModule::ShutdownModule()
{
#if WITH_EDITOR
	FCoreDelegates::OnPostEngineInit.Remove(PostEngineInitHandle);
	if (FAssetRegistryModule* AssetRegistryModule = FModuleManager::GetModulePtr<FAssetRegistryModule>(TEXT("AssetRegistry")))
	{
		AssetRegistryModule->Get().OnFilesLoaded().Remove(FilesLoadedHandle);
	}
#endif

	Super::ShutdownModule();
}

#if WITH_EDITOR
void FYourProjectGameModule::LoadBlueprintSubsystems()
{
	// One-shot: never run twice.
	FCoreDelegates::OnPostEngineInit.Remove(PostEngineInitHandle);

	IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get();

	// The registry may still be discovering files at this point (background scan).
	// Bind first, then check — this avoids missing the broadcast in a race.
	FilesLoadedHandle = AssetRegistry.OnFilesLoaded().AddRaw(this, &FYourProjectGameModule::LoadBlueprintSubsystemsNow);
	if (!AssetRegistry.IsLoadingAssets())
	{
		AssetRegistry.OnFilesLoaded().Remove(FilesLoadedHandle);
		LoadBlueprintSubsystemsNow();
	}
	// Otherwise LoadBlueprintSubsystemsNow runs when initial discovery finishes.
}

void FYourProjectGameModule::LoadBlueprintSubsystemsNow()
{
	if (FAssetRegistryModule* AssetRegistryModule = FModuleManager::GetModulePtr<FAssetRegistryModule>(TEXT("AssetRegistry")))
	{
		AssetRegistryModule->Get().OnFilesLoaded().Remove(FilesLoadedHandle);
	}

	FARFilter Filter;
	Filter.ClassPaths.Add(UBlueprint::StaticClass()->GetClassPathName());
	Filter.bRecursiveClasses = true;
	Filter.bRecursivePaths = true;

	TArray<FAssetData> BlueprintAssets;
	FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get().GetAssets(Filter, BlueprintAssets);

	int32 LoadedCount = 0;
	for (const FAssetData& Asset : BlueprintAssets)
	{
		// NativeParentClass tag holds e.g. "/Script/YourProject.SubsystemGameInstance".
		FString NativeParentClass;
		if (!Asset.GetTagValue(FBlueprintTags::NativeParentClassPath, NativeParentClass) ||
			!NativeParentClass.Contains(TEXT("SubsystemGameInstance")))
		{
			continue;
		}

		if (const UBlueprint* Blueprint = Cast<UBlueprint>(Asset.GetAsset()))
		{
			if (Blueprint->GeneratedClass && Blueprint->GeneratedClass->IsChildOf(USubsystemGameInstance::StaticClass()))
			{
				++LoadedCount;
				UE_LOG(LogTemp, Log, TEXT("YourProject: loaded GameInstance Subsystem %s"), *Blueprint->GeneratedClass->GetName());
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("YourProject: loaded %d Blueprint GameInstance Subsystem(s)"), LoadedCount);
}
#endif

IMPLEMENT_PRIMARY_GAME_MODULE( FYourProjectGameModule, YourProject, "YourProject" );
```

**Note:** if your game module class is not `FDefaultGameModuleImpl`-based, derive from your existing
module class instead and keep the `IMPLEMENT_*_MODULE` macro you already have (only the class name changes).

## 3. Module dependencies (`Source/YourProject/YourProject.Build.cs`)

Add `AssetRegistry` to the private dependencies:

```csharp
PrivateDependencyModuleNames.AddRange(new string[] { "AssetRegistry" });
```

## 4. Verify

Compile, restart the editor, open the Output Log: you should see
`YourProject: loaded GameInstance Subsystem BP_...` lines, and every subsystem
has its dedicated `Get ...` node without opening anything by hand.
