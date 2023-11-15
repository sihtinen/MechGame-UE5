// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MechEquipmentComponentRTBase.h"
#include "MechDataStructures.h"
#include "EquipmentWidgetSourceInterface.h"
#include "MechWeaponComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MECHGAME_API UMechWeaponComponent : public UMechEquipmentComponentRTBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<class UMechProjectileWeaponAsset> SettingsAsset;

	UPROPERTY(BlueprintReadOnly)
	bool bInputActive = false;

public:	

	UMechWeaponComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupGameplay(class AMech* Mech, class UMechProjectileWeaponAsset* WeaponAsset, const EEquipmentSlotType& Slot);

	UFUNCTION(BlueprintCallable)
	bool IsAiming();

	FName GetDisplayName() override;

	int32 GetMaxUseCount() override;

	int32 GetRemainingUseCount() override;

	bool IsAccuracyVisibleInWidget() override
	{
		return true;
	}

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnInputSlotStateUpdated(const EEquipmentSlotType& SlotType, bool IsPressed);

	FVector GetShootDirection(const FVector& ShootLocation);

	void Shoot(const double& TimeElapsedPreviouslyFired, const double& WaitTimeBetweenShots);

private:

	bool bFiredPreviousTick = false;

	double TimePreviouslyFired;

	int32 NumPendingShots = 0;
};
