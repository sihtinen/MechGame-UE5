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

	bool bDrawTraceDebug = false;

	void Initialize(FSubsystemCollectionBase& Collection) override;

	void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override
	{
		return GetStatID();
	}

	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(AActor* SourceActor, UProjectileAsset* ProjectileAsset, const FVector& Location, const FVector& Direction, class UContextTargetComponent* TargetComponent = nullptr);

private:

	FHitResult TraceHitResult;

	TArray<FProjectileState> ActiveProjectiles;

	TArray<AActor*> TraceIgnoredActors;

private:

	/**
	Updates projectile state
	Returns TRUE if projectile is alive
	FALSE if projectile has collided or life time has expired
	*/
	bool UpdateProjectile(FProjectileState& Projectile, const float& DeltaTime);

	void InitializeProjectileVisualActor(UProjectileAsset* ProjectileAsset, FProjectileState& NewProjectile);
};
