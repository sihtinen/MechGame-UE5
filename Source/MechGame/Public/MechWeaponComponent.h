// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MechDataStructures.h"
#include "MechWeaponComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MECHGAME_API UMechWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<class UMechProjectileWeaponAsset> SettingsAsset;

	UPROPERTY(BlueprintReadOnly)
	EEquipmentSlotType Slot;

public:	

	UMechWeaponComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupGameplay(class AMech* Mech, class UMechProjectileWeaponAsset* WeaponAsset, const EEquipmentSlotType& Slot);

	UFUNCTION(BlueprintCallable)
	bool IsAiming();

protected:
	UPROPERTY()
	TWeakObjectPtr<class AMech> Mech;

protected:
	virtual void BeginPlay() override;
};
