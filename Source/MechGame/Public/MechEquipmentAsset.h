// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MechDataStructures.h"
#include "MechEquipmentAsset.generated.h"

UCLASS(BlueprintType)
class MECHGAME_API UMechEquipmentAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSoftObjectPtr<class USkeletalMesh> Mesh;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UAnimInstance> AnimBlueprint;

public:
	virtual void SetupMechRuntime(class AMech* Mech, const EEquipmentSlotType& Slot);
};
