// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ContextSubsystem.generated.h"

class UContextTargetComponent;

UCLASS()
class MECHGAME_API UContextSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void RegisterContextTargetComponent(UContextTargetComponent* Component);
	void UnregisterContextTargetComponent(UContextTargetComponent* Component);

	void GetContextTargetsOnLayers(TArray<TWeakObjectPtr<UContextTargetComponent>>* ArrayToPopulate, const int32& QueryLayers);

	UFUNCTION(BlueprintCallable)
	static bool BitMaskIncludes(const int32& BitMask, const int32& Layer);

private:
	TArray<TWeakObjectPtr<UContextTargetComponent>> ActiveContextTargetComponents;
};
