// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EquipmentWidgetSourceInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UEquipmentWidgetSourceInterface : public UInterface
{
	GENERATED_BODY()
};

class MECHGAME_API IEquipmentWidgetSourceInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	const FName GetDisplayName();
	virtual const FName GetDisplayName_Implementation() { return FName(); }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetMaxUseCount();
	virtual int32 GetMaxUseCount_Implementation() { return 0; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetRemainingUseCount();
	virtual int32 GetRemainingUseCount_Implementation() { return 0; }

	UFUNCTION(BlueprintImplementableEvent)
	void OnUseCountUpdated(const int32& RemainingUseCount);
};
