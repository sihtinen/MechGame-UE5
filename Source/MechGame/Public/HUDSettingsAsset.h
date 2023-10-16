// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HUDSettingsAsset.generated.h"

UCLASS(BlueprintType)
class MECHGAME_API UHUDSettingsAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetingAreaScreenHeightOffset;
};
