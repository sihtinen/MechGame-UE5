// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsUtils.h"
#include "PIDAsset.h"

FPIDState PhysicsUtils::ApplyAngleTorquePID(
	TObjectPtr<UPIDAsset> PIDAsset, 
	TObjectPtr<UPrimitiveComponent> PrimitiveComponent,
	FPIDState PIDState,
	FVector RotateAxis,
	float DeltaTime,
	float CurrentAngle,
	float TargetAngle)
{
	PIDState = PIDAsset->UpdateAngleTick(PIDState, DeltaTime, CurrentAngle, TargetAngle);
	FVector Torque = PIDState.Output * RotateAxis;
	PrimitiveComponent->AddTorqueInRadians(Torque, NAME_None, true);
	return PIDState;
}
