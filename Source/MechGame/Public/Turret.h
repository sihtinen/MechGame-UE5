// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MechDataStructures.h"
#include "Kismet/KismetMathLibrary.h"
#include "Turret.generated.h"

class UContextTargetComponent;

UCLASS()
class MECHGAME_API ATurret : public APawn
{
	GENERATED_BODY()
	
public:	

	ATurret();

	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UMechProjectileWeaponAsset> WeaponAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = EContextLayers))
	int32 TargetContextLayerFlags = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	double MaxTargetingDistance = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRuntimeFloatCurve AccuracyOverDistanceCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRuntimeFloatCurve PredictionOverDistanceCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ValidShootDirectionDot = 0.7f;

	UPROPERTY()
	TArray<TWeakObjectPtr<UContextTargetComponent>> ValidTargetsArray;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UContextTargetComponent> CurrentTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SelectTargetUpdateRate = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bLimitRotation = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RotationLimitPitch = 35.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RotationLimitYaw = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RotationSpringStiffness = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RotationSpringCriticalDamping = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRuntimeFloatCurve ScoreOverDistanceCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRuntimeFloatCurve ScoreOverDefaultDirectionDot;

	UPROPERTY(BlueprintReadWrite)
	FRotator DefaultRotator;

	UPROPERTY()
	TArray<AActor*> TraceIgnoredActors;

	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<class USceneComponent> RotationRootComponent;

	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<class USceneComponent> ProjectileSpawnTransformComponent;

	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<class UContextTargetComponent> MyContextTargetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bProjectilesDrawDebug = false;

protected:

	virtual void BeginPlay() override;

	void SelectTarget();

	bool IsTargetValid(const TWeakObjectPtr<UContextTargetComponent>& Target);

	float CalculateTargetScore(const TWeakObjectPtr<UContextTargetComponent>& Target);

	void UpdateRotation(float DeltaTime);

	void UpdateShootLogic(float DeltaTime);

	FVector GetShootDirection(const FVector& ShootLocation, const int& IterationThisTick);

private:

	bool bFiredPreviousTick = false;

	bool bRotationValidToShoot = false;

	double TimePreviouslyFired;

	float SelectTargetRemainingWaitTime;

	int32 NumPendingShots = 0;

	FQuaternionSpringState RotationSpringState = FQuaternionSpringState();

};
