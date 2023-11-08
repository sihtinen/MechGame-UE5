// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MechDataStructures.h"
#include "EquipmentWidgetSourceInterface.h"
#include "MechWeaponComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MECHGAME_API UMechWeaponComponent : public UActorComponent, public IEquipmentWidgetSourceInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<class UMechProjectileWeaponAsset> SettingsAsset;

	UPROPERTY(BlueprintReadOnly)
	bool bInputActive = false;

	UPROPERTY(BlueprintReadOnly)
	EEquipmentSlotType Slot = EEquipmentSlotType::MAX;

public:	

	UMechWeaponComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupGameplay(class AMech* Mech, class UMechProjectileWeaponAsset* WeaponAsset, const EEquipmentSlotType& Slot);

	UFUNCTION(BlueprintCallable)
	bool IsAiming();

//IEquipmentWidgetSourceInterface implementation
public:

	UFUNCTION()
	const FName GetDisplayName_Implementation() override;

	UFUNCTION()
	int32 GetMaxUseCount_Implementation() override;

	UFUNCTION()
	int32 GetRemainingUseCount_Implementation() override;

protected:

	UPROPERTY()
	TWeakObjectPtr<class AMech> Mech;

	int32 RemainingUseCount;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnInputSlotStateUpdated(const EEquipmentSlotType& SlotType, bool IsPressed);

	FVector GetShootDirection(const FVector& ShootLocation);

private:

	bool bFiredPreviousTick = false;

	double TimePreviouslyFired;
};
