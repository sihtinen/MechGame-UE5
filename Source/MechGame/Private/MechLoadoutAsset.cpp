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

void UMechLoadoutAsset::TryAddEquipmentAssetSoftObjectPath(UMechEquipmentAsset* Asset)
{
	if (!Asset)
	{
		int32 ID = GetUniqueID();

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(ID, 15.f, FColor::Orange, TEXT("MechLoadoutAsset::GetValidAssetSoftObjectPaths(): Asset null!"), false);

		UE_LOG(LogTemp, Warning, TEXT("MechLoadoutAsset::GetValidAssetSoftObjectPaths(): Asset null!"));

		return;
	}

	if (Asset->Mesh.IsNull())
	{
		int32 ID = GetUniqueID();
		FString AssetName = Asset->GetName();

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(ID, 15.f, FColor::Orange, FString::Printf(TEXT("MechLoadoutAsset::GetValidAssetSoftObjectPaths(): Asset %s mesh is null!"), *AssetName), false);
		
		UE_LOG(LogTemp, Warning, TEXT("MechLoadoutAsset::GetValidAssetSoftObjectPaths(): Asset %s mesh is null!"), *AssetName);

		return;
	}

	AssetSoftObjectPaths.Add(Asset->Mesh.ToSoftObjectPath());
}