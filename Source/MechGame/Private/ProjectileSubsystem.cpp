// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileSubsystem.h"
#include "Logging/StructuredLog.h"
#include "ProjectileAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PooledActor.h"
#include "Engine/World.h"
#include "ActorPoolSubsystem.h"
#include "ContextTargetComponent.h"
#include "ActorPool.h"

void UProjectileSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ActiveProjectiles.Reserve(1024);
	TraceIgnoredActors.Add(nullptr);
}

void UProjectileSubsystem::Tick(float DeltaTime)
{
	int32 ActiveProjectilesCount = ActiveProjectiles.Num();

	if (ActiveProjectilesCount == 0)
		return;

	for (int32 i = ActiveProjectilesCount; i --> 0;)
	{
		FProjectileState& Projectile = ActiveProjectiles[i];

		bool bProjectileAlive = UpdateProjectile(Projectile, DeltaTime);

		if (bProjectileAlive == false)
		{
			if (Projectile.VisualActor.IsValid())
			{
				Projectile.VisualActor->Deactivate();
				Projectile.VisualActor->ReturnToPool();
				Projectile.VisualActor = nullptr;
			}

			Projectile.TargetComponent = nullptr;

			ActiveProjectiles.RemoveAt(i);
		}
	}
}

void UProjectileSubsystem::SpawnProjectile(AActor* SourceActor, UProjectileAsset* ProjectileAsset, const FVector& Location, const FVector& Direction, UContextTargetComponent* TargetComponent)
{
	if (SourceActor == nullptr)
	{
		UE_LOGFMT(LogTemp, Error, "UProjectileSubsystem::SpawnProjectile(): SourceActor parameter is null pointer - exiting early");
		return;
	}

	if (ProjectileAsset == nullptr)
	{
		UE_LOGFMT(LogTemp, Error, "UProjectileSubsystem::SpawnProjectile(): ProjectileAsset parameter is null pointer - exiting early");
		return;
	}

	FProjectileState NewProjectile = FProjectileState(SourceActor, ProjectileAsset);
	NewProjectile.Location = Location;
	NewProjectile.ForwardDirection = Direction;
	NewProjectile.Velocity = ProjectileAsset->InitialSpeed * Direction;
	NewProjectile.TargetComponent = TargetComponent;

	InitializeProjectileVisualActor(ProjectileAsset, NewProjectile);

	ActiveProjectiles.Add(NewProjectile);
}

void UProjectileSubsystem::InitializeProjectileVisualActor(UProjectileAsset* ProjectileAsset, FProjectileState& NewProjectile)
{
	if (ProjectileAsset->VisualActorSubclass == nullptr)
		return;

	UActorPoolSubsystem* ActorPoolSubsystem = GetWorld()->GetSubsystem<UActorPoolSubsystem>();

	if (ActorPoolSubsystem == nullptr)
		return;

	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(FVector::ZeroVector, NewProjectile.ForwardDirection);
	NewProjectile.VisualActor = ActorPoolSubsystem->GetPooledActor(ProjectileAsset->VisualActorSubclass, false);
	NewProjectile.VisualActor->SetActorTransform(FTransform(Rotation, NewProjectile.Location));
	NewProjectile.VisualActor->Activate();
}

bool UProjectileSubsystem::UpdateProjectile(FProjectileState& Projectile, const float& DeltaTime)
{
	const UProjectileAsset* Settings = Projectile.ProjectileAsset.Get();

	if (Settings == nullptr)
		return false;

	const FVector PrevLocation = Projectile.Location;

	Projectile.Location += DeltaTime * Projectile.Velocity;

	const ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_PhysicsBody);
	const bool TraceAgainstComplexGeo = false;
	const bool IgnoreSelf = true;
	const EDrawDebugTrace::Type DrawDebugType = (bDrawTraceDebug ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None);

	TraceIgnoredActors[0] = Projectile.OwnerActor.Get();

	const bool bTraceHitFound = UKismetSystemLibrary::SphereTraceSingle(
		this,
		PrevLocation,
		Projectile.Location,
		Settings->CollisionRadius,
		TraceChannel,
		TraceAgainstComplexGeo,
		TraceIgnoredActors,
		DrawDebugType,
		TraceHitResult,
		IgnoreSelf);

	TraceIgnoredActors[0] = nullptr;

	if (bTraceHitFound)
	{
		if (Projectile.VisualActor.IsValid())
		{
			FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(FVector::ZeroVector, Projectile.ForwardDirection);
			Projectile.VisualActor->SetActorTransform(FTransform(Rotation, TraceHitResult.ImpactPoint));
		}

		return false;
	}

	Projectile.AliveTime += DeltaTime;

	if (Projectile.VisualActor.IsValid())
	{
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(FVector::ZeroVector, Projectile.ForwardDirection);
		Projectile.VisualActor->SetActorTransform(FTransform(Rotation, Projectile.Location));
	}

	const bool bProjectileAlive = (Projectile.AliveTime < Settings->LifeTime);

	return bProjectileAlive;
}
