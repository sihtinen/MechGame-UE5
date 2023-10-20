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
	void UpdateTick(FPIDState& State, const float& DeltaTime, const float& CurrentValue, const float& TargetValue);

	UFUNCTION(BlueprintCallable)
	void UpdateAngleTick(FPIDState& State, const float& DeltaTime, const float& CurrentAngle, const float& TargetAngle);

private:

	float AngleDifference(const float& a, const float& b);
};