// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PIDState.generated.h"

USTRUCT(BlueprintType)
struct FPIDState
{
	GENERATED_BODY()

public:

	bool bDerivativeInitialized;

	float PrevError;
	float PrevValue;
	float IntegrationStored;
	float Output;
	float P, I, D;

public:

	void ResetState();
};

UENUM(BlueprintType)
enum class FDerivativeMeasurementType : uint8
{
	Velocity = 0 UMETA(DisplayName = "Velocity"),
	ErrorChangeRate = 1 UMETA(DisplayName = "Error Change Rate"),
};