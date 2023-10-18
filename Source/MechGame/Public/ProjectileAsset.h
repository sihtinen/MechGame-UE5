// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProjectileAsset.generated.h"

UCLASS(BlueprintType)
class MECHGAME_API UProjectileAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InitialSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HorizontalDrag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float VerticalDrag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GravityForce;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CollisionRadius;
};
