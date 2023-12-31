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

	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(
		APawn* SourcePawn, 
		UProjectileAsset* ProjectileAsset, 
		const FVector& Location, 
		const FVector& Direction, 
		class UContextTargetComponent* TargetComponent = nullptr, 
		bool DrawDebug = false);

	UFUNCTION(BlueprintCallable)
	static void CalculateInterceptDirection(
		const FVector& SourceLocation,
		const FVector& SourceVelocity,
		const FVector& TargetLocation,
		const FVector& TargetVelocity,
		const float ProjectileSpeed,
		const float DragCoefficient,
		const float Gravity,
		bool& bResultFound,
		FVector& ResultDirection);

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
