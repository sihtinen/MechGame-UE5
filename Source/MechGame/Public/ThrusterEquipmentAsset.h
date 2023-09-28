// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BoostParameters.h"
#include "ThrusterEquipmentAsset.generated.h"

UCLASS()
class MECHGAME_API UThrusterEquipmentAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBoostParameters SustainedBoostParams_Horizontal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBoostParameters SustainedBoostParams_Vertical;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SustainedBoostEnergyDrainRate;

	// ---

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBoostParameters DashBoostParams_Horizontal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBoostParameters DashBoostParams_Vertical;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DashEnergyDrain;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DashPrepareTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DashLength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DashVerticalVelocityCancelAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRuntimeFloatCurve DashForceCurve;

public:
	FBoostParameters GetSustainedBoostParams(float inputAmount);
	FBoostParameters GetDashBoostParams(float inputAmount);
};