// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PIDAsset.h"
#include "PIDState.h"

class MECHGAME_API PhysicsUtils
{
public:
	static FPIDState ApplyAngleTorquePID(
		TObjectPtr<UPIDAsset> PIDAsset,
		TObjectPtr<UPrimitiveComponent> PrimitiveComponent,
		FPIDState PIDState,
		FVector RotateAxis,
		float DeltaTime,
		float CurrentAngle,
		float TargetAngle);
};
