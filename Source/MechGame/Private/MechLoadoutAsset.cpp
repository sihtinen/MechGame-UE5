// Fill out your copyright notice in the Description page of Project Settings.


#include "MechLoadoutAsset.h"
#include "MechEquipmentAsset.h"

TArray<FSoftObjectPath> UMechLoadoutAsset::GetValidAssetSoftObjectPaths()
{
	AssetSoftObjectPaths.Reset();

	TryAddEquipmentAssetSoftObjectPath(HeadAsset);
	TryAddEquipmentAssetSoftObjectPath(BodyAsset);
	TryAddEquipmentAssetSoftObjectPath(ArmsAsset);
	TryAddEquipmentAssetSoftObjectPath(LegsAsset);

	return AssetSoftObjectPaths;
}

void UMechLoadoutAsset::TryAddEquipmentAssetSoftObjectPath(TObjectPtr<UMechEquipmentAsset> Asset)
{
	if (Asset && Asset->Mesh && Asset->Mesh.IsValid())
		AssetSoftObjectPaths.Add(Asset->Mesh.ToSoftObjectPath());
}