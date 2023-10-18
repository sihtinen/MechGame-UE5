// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MechDataStructures.h"
#include "ProjectileSubsystem.generated.h"

UCLASS()
class MECHGAME_API UProjectileSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
public:

	void Initialize(FSubsystemCollectionBase& Collection) override;

	void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override
	{
		return GetStatID();
	}

private:

	TArray<FProjectileState> ActiveProjectiles;
};
