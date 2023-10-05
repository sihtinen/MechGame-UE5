// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MechEquipmentAsset.generated.h"

class USkeletalMesh;

UCLASS(BlueprintType)
class MECHGAME_API UMechEquipmentAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSoftObjectPtr<USkeletalMesh> Mesh;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UAnimInstance> AnimBlueprint;
};
