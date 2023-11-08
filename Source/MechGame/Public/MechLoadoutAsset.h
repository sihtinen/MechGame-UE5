// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MechDataStructures.h"
#include "MechLoadoutAsset.generated.h"

class UMechEquipmentAsset;

UCLASS(BlueprintType)
class MECHGAME_API UMechLoadoutAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<EEquipmentSlotType, TObjectPtr<UMechEquipmentAsset>> Slots;

public:
	UMechLoadoutAsset();

	const TArray<FSoftObjectPath>& GetValidAssetSoftObjectPaths();

private:
	UPROPERTY()
	TArray<FSoftObjectPath> AssetSoftObjectPaths;

private:
	void TryAddEquipmentAssetSoftObjectPath(UMechEquipmentAsset* Asset);
};
