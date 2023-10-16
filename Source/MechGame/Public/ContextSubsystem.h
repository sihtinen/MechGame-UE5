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

private:
	TArray<TWeakObjectPtr<UContextTargetComponent>> ActiveContextTargetComponents;
};
