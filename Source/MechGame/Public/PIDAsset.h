// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PIDState.h"
#include "PIDAsset.generated.h"

UCLASS(BlueprintType)
class MECHGAME_API UPIDAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProportionalGain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float IntegralGain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float IntegralLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DerivativeGain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OutputMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OutputMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDerivativeMeasurementType DerivativeMeasurementType;

public:

	UFUNCTION(BlueprintCallable)
	FPIDState UpdateTick(FPIDState State, float DeltaTime, float CurrentValue, float TargetValue);

	UFUNCTION(BlueprintCallable)
	FPIDState UpdateAngleTick(FPIDState State, float DeltaTime, float CurrentAngle, float TargetAngle);

private:

	float AngleDifference(float a, float b);
};