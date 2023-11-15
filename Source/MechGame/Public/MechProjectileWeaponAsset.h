// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MechEquipmentAsset.h"
#include "MechProjectileWeaponAsset.generated.h"

UCLASS(BlueprintType)
class MECHGAME_API UMechProjectileWeaponAsset : public UMechEquipmentAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	int32 UseCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	float UseRatePerSecond = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	bool bBurstFire = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	int32 BurstNumShots = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	float BurstCooldownTime = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	FAccuracyConfig AccuracyConfig = FAccuracyConfig();

	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	FWeaponInaccuracyNoise InaccuracyNoise = FWeaponInaccuracyNoise();

	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	TObjectPtr<class UProjectileAsset> ProjectileAsset;

	void SetupMechRuntime(class AMech* Mech, const EEquipmentSlotType& Slot) override;
};
