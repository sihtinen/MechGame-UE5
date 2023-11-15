// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MechDataStructures.h"
#include "MechEquipmentComponentRTBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUseCountUpdated, int32, CurrentRemainingUseCount, int32, PreviousRemainingUseCount);

UCLASS(ClassGroup=(Custom))
class MECHGAME_API UMechEquipmentComponentRTBase : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	EEquipmentSlotType Slot = EEquipmentSlotType::MAX;

	UPROPERTY(BlueprintAssignable)
	FOnUseCountUpdated OnUseCountUpdated;

public:	

	UMechEquipmentComponentRTBase();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual FName GetDisplayName();

	UFUNCTION(BlueprintCallable)
	virtual int32 GetMaxUseCount();

	UFUNCTION(BlueprintCallable)
	virtual int32 GetRemainingUseCount();

	UFUNCTION(BlueprintCallable)
	virtual bool IsAccuracyVisibleInWidget()
	{
		return false;
	}

	UFUNCTION(BlueprintCallable)
	float GetAccuracy()
	{
		return Accuracy;
	}

protected:

	UPROPERTY()
	TWeakObjectPtr<class AMech> Mech;

	int32 RemainingUseCount;

	float Accuracy;

protected:

	virtual void BeginPlay() override;
};
