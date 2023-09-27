// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MechPhysicsAsset.generated.h"

/**
 * 
 */
UCLASS()
class MECHGAME_API UMechPhysicsAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CollisionCapsuleRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CollisionCapsuleHalfHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SurfaceRideHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SurfaceTraceHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SurfaceTraceDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SurfaceTraceRadius;
};
