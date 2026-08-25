#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SubsystemGameInstance.generated.h"

/**
 * Base GameInstanceSubsystem class with Blueprint support,
 * lifecycle events, and a dynamic type-safe getter node.
 * 
 * ⚠️ IMPORTANT: Replace KINGDOMOFISRION_API with your project/module API macro (e.g., YOURPROJECT_API).
 */
UCLASS(Blueprintable, BlueprintType)
class KINGDOMOFISRION_API USubsystemGameInstance : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// C++ lifecycle overrides
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Blueprint lifecycle events (overrideable in BP under Functions -> Override)
	UFUNCTION(BlueprintImplementableEvent, Category = "Subsystem", meta = (DisplayName = "On Initialize"))
	void ReceiveInitialize();

	UFUNCTION(BlueprintImplementableEvent, Category = "Subsystem", meta = (DisplayName = "On Deinitialize"))
	void ReceiveDeinitialize();

	/** Returns the instance of the specified Game Instance Subsystem class, automatically casting the output pin. */
	UFUNCTION(BlueprintPure, Category = "Subsystems", meta = (WorldContext = "WorldContextObject", DeterminesOutputType = "SubsystemClass", ToolTip = "Returns the instance of the specified Game Instance Subsystem class."))
	static USubsystemGameInstance* GetCustomSubsystem(const UObject* WorldContextObject, TSubclassOf<USubsystemGameInstance> SubsystemClass);
};
