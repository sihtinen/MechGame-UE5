// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MechLoadoutAsset.generated.h"

class UMechEquipmentAsset;

UCLASS()
class MECHGAME_API UMechLoadoutAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UMechEquipmentAsset> HeadAsset;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UMechEquipmentAsset> BodyAsset;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UMechEquipmentAsset> ArmsAsset;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UMechEquipmentAsset> LegsAsset;

public:
	TArray<FSoftObjectPath> GetValidAssetSoftObjectPaths();

private:
	UPROPERTY()
	TArray<FSoftObjectPath> AssetSoftObjectPaths;

private:
	void TryAddEquipmentAssetSoftObjectPath(TObjectPtr<UMechEquipmentAsset> Asset);
};
