#include "SubsystemGameInstance.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

void USubsystemGameInstance::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ReceiveInitialize();
}

void USubsystemGameInstance::Deinitialize()
{
	ReceiveDeinitialize();
	Super::Deinitialize();
}

USubsystemGameInstance* USubsystemGameInstance::GetCustomSubsystem(const UObject* WorldContextObject, TSubclassOf<USubsystemGameInstance> SubsystemClass)
{
	if (!WorldContextObject || !SubsystemClass)
	{
		return nullptr;
	}

	UWorld* World = WorldContextObject->GetWorld();
	if (!World)
	{
		return nullptr;
	}

	UGameInstance* GI = World->GetGameInstance();
	if (!GI)
	{
		return nullptr;
	}

	return Cast<USubsystemGameInstance>(GI->GetSubsystemBase(SubsystemClass));
}
