// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoostParameters.generated.h"

USTRUCT(BlueprintType)
struct FBoostParameters
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HorizontalForce;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float VerticalForce;

public:
	static FBoostParameters Lerp(const FBoostParameters& a, const FBoostParameters& b, const float& t);
};