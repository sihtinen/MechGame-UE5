// Fill out your copyright notice in the Description page of Project Settings.

#include "Turret.h"
#include "ContextSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "ContextTargetComponent.h"
#include "ProjectileSubsystem.h"
#include "MechProjectileWeaponAsset.h"
#include "ProjectileAsset.h"
#include "Logging/StructuredLog.h"

ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
	DefaultRotator = GetActorRotation();

	SelectTargetRemainingWaitTime = FMath::RandRange(0.0f, SelectTargetUpdateRate);

	TraceIgnoredActors.Add(this);
	TraceIgnoredActors.Add(nullptr);
}

void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WeaponAsset == false || RotationRootComponent.IsValid() == false)
		return;

	SelectTargetRemainingWaitTime -= DeltaTime;

	if (SelectTargetRemainingWaitTime <= 0)
	{
		SelectTargetRemainingWaitTime += SelectTargetUpdateRate;
		SelectTarget();
	}

	UpdateRotation(DeltaTime);

	UpdateShootLogic(DeltaTime);
}

void ATurret::SelectTarget()
{
	auto GameInstance = UGameplayStatics::GetGameInstance(this);

	if (GameInstance == nullptr)
		return;

	auto ContextSubsystem = GameInstance->GetSubsystem<UContextSubsystem>();

	if (ContextSubsystem == nullptr)
		return;

	ContextSubsystem->GetContextTargetsOnLayers(&ValidTargetsArray, TargetContextLayerFlags);

	if (ValidTargetsArray.Num() == 0)
	{
		CurrentTarget = nullptr;
		ValidTargetsArray.Reset();
		return;
	}

	UContextTargetComponent* BestTarget = nullptr;
	float BestScore = -100.0f;

	for (int32 i = 0; i < ValidTargetsArray.Num(); i++)
	{
		auto Target = ValidTargetsArray[i];

		if (IsTargetValid(Target) == false)
		{
			continue;
		}

		float Score = CalculateTargetScore(Target);

		if (Score > BestScore)
		{
			BestTarget = Target.Get();
			BestScore = Score;
		}
	}

	CurrentTarget = BestTarget;

	ValidTargetsArray.Reset();
}

bool ATurret::IsTargetValid(const TWeakObjectPtr<UContextTargetComponent>& Target)
{
	if (Target.IsValid() == false)
	{
		return false;
	}

	UContextTargetComponent* TargetPtr = Target.Get();
	FVector ToTargetVector = TargetPtr->GetComponentLocation() - GetActorLocation();

	if (ToTargetVector.SquaredLength() > (MaxTargetingDistance * MaxTargetingDistance))
	{
		return false;
	}

	if (bLimitRotation)
	{
		FRotator MyRotator = RotationRootComponent->GetComponentRotation().GetNormalized();
		FRotator ToTargetRotator = ToTargetVector.GetSafeNormal().ToOrientationRotator();

		float PitchDifference = FMath::Abs(ToTargetRotator.Pitch - MyRotator.Pitch);

		if (PitchDifference > RotationLimitPitch)
		{
			return false;
		}

		float YawDifference = FMath::Abs(ToTargetRotator.Yaw - MyRotator.Yaw);

		if (YawDifference > RotationLimitYaw)
		{
			return false;
		}
	}

	TraceIgnoredActors[1] = TargetPtr->GetOwner();

	FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams::DefaultQueryParam;
	CollisionQueryParams.AddIgnoredActors(TraceIgnoredActors);

	bool bObstructionFound = GetWorld()->LineTraceTestByChannel(
		GetActorLocation(),
		TargetPtr->GetComponentLocation(),
		ECollisionChannel::ECC_PhysicsBody,
		CollisionQueryParams);

	TraceIgnoredActors[1] = nullptr;

	if (bObstructionFound)
	{
		return false;
	}

	return true;
}

float ATurret::CalculateTargetScore(const TWeakObjectPtr<UContextTargetComponent>& Target)
{
	UContextTargetComponent* TargetPtr = Target.Get();

	FVector ToTargetVector = TargetPtr->GetComponentLocation() - GetActorLocation();
	float DistanceToTarget = ToTargetVector.Length();

	float DistanceScore = ScoreOverDistanceCurve.GetRichCurve()->Eval(DistanceToTarget / MaxTargetingDistance);

	float DefaultDirectionDot = FVector::DotProduct(ToTargetVector.GetSafeNormal(), DefaultRotator.Vector());
	float DefaultDirectionDotScore = ScoreOverDefaultDirectionDot.GetRichCurve()->Eval(DefaultDirectionDot);

	return DistanceScore + DefaultDirectionDotScore;
}

void ATurret::UpdateRotation(float DeltaTime)
{
	FQuat TargetQuaternion = DefaultRotator.Quaternion();

	if (CurrentTarget.IsValid())
	{
		FVector SpawnLocation = (ProjectileSpawnTransformComponent.IsValid() ? 
			ProjectileSpawnTransformComponent->GetComponentLocation() : 
			RotationRootComponent->GetComponentLocation());

		FVector MyVelocity = (MyContextTargetComponent.IsValid() ? MyContextTargetComponent->GetCalculatedVelocity() : FVector::Zero());

		bool bInterceptDirectionFound = false;
		FVector InterceptDirection;

		UProjectileSubsystem::CalculateInterceptDirection(
			SpawnLocation,
			MyVelocity,
			CurrentTarget->GetComponentLocation(),
			CurrentTarget->GetCalculatedVelocity(),
			WeaponAsset->ProjectileAsset->InitialSpeed,
			WeaponAsset->ProjectileAsset->DragCoefficient,
			WeaponAsset->ProjectileAsset->GravityForce,
			bInterceptDirectionFound,
			InterceptDirection);

		if (bInterceptDirectionFound)
		{
			FVector ToTarget = CurrentTarget->GetComponentLocation() - SpawnLocation;

			float PredictionAmount = PredictionOverDistanceCurve.GetRichCurve()->Eval(ToTarget.Length() / MaxTargetingDistance);

			InterceptDirection = FMath::Lerp(ToTarget.GetSafeNormal(), InterceptDirection, PredictionAmount).GetSafeNormal();

			TargetQuaternion = InterceptDirection.ToOrientationQuat();
		}
	}

	FQuat NewQuaternion = UKismetMathLibrary::QuaternionSpringInterp(
		RotationRootComponent->GetComponentQuat(),
		TargetQuaternion,
		RotationSpringState,
		RotationSpringStiffness,
		RotationSpringCriticalDamping,
		DeltaTime);

	RotationRootComponent->SetWorldRotation(NewQuaternion);

	if (CurrentTarget.IsValid())
	{
		float DotToTargetDirection = FVector::DotProduct(RotationRootComponent->GetForwardVector(), TargetQuaternion.Vector());
		bRotationValidToShoot = (DotToTargetDirection >= ValidShootDirectionDot);
	}
	else
		bRotationValidToShoot = false;
}

void ATurret::UpdateShootLogic(float DeltaTime)
{
	if (bRotationValidToShoot == false || CurrentTarget.IsValid() == false)
		return;

	const double TimeNow = UGameplayStatics::GetTimeSeconds(this);
	const double WaitTimeBetweenShots = 1.0 / WeaponAsset->UseRatePerSecond;
	double TimeElapsedPreviouslyFired = TimeNow - TimePreviouslyFired;

	if (WeaponAsset->bBurstFire)
	{
		if (NumPendingShots > 0)
		{
			if (TimeElapsedPreviouslyFired < WaitTimeBetweenShots)
				return;
		}
		else
		{
			if (TimeElapsedPreviouslyFired < WeaponAsset->BurstCooldownTime)
				return;

			NumPendingShots = WeaponAsset->BurstNumShots;

			if (NumPendingShots <= 0)
				NumPendingShots = 1;

			TimeElapsedPreviouslyFired = WaitTimeBetweenShots;
		}
	}
	else
	{
		if (TimeElapsedPreviouslyFired < WaitTimeBetweenShots)
			return;

		NumPendingShots = 1;
	}

	bFiredPreviousTick = true;
	TimePreviouslyFired = TimeNow;

	UWorld* World = GetWorld();

	if (World == nullptr)
	{
		NumPendingShots = 0;
		return;
	}

	UProjectileSubsystem* ProjectileSubsystem = World->GetSubsystem<UProjectileSubsystem>();

	if (ProjectileSubsystem == nullptr)
	{
		NumPendingShots = 0;
		return;
	}

	float RemainingTime = TimeElapsedPreviouslyFired;
	int32 NumSpawned = 0;
	FVector ProjectileSpawnLocation = (ProjectileSpawnTransformComponent.IsValid() ? ProjectileSpawnTransformComponent->GetComponentLocation() : GetActorLocation());

	while (RemainingTime >= WaitTimeBetweenShots)
	{
		RemainingTime -= WaitTimeBetweenShots;

		if (NumPendingShots <= 0)
		{
			NumPendingShots = 0;
			break;
		}

		FVector InstanceSpawnLocation = ProjectileSpawnLocation;
		FVector ShootDirection = GetShootDirection(ProjectileSpawnLocation, NumSpawned);

		if (NumSpawned > 0)
			InstanceSpawnLocation += (NumSpawned * WaitTimeBetweenShots) * WeaponAsset->ProjectileAsset->InitialSpeed * ShootDirection;

		ProjectileSubsystem->SpawnProjectile(
			this,
			WeaponAsset->ProjectileAsset,
			InstanceSpawnLocation,
			ShootDirection,
			CurrentTarget.Get(),
			bProjectilesDrawDebug);

		NumPendingShots--;
		NumSpawned++;
	}
}

FVector ATurret::GetShootDirection(const FVector& ShootLocation, const int& IterationThisTick)
{
	FVector ResultDirection = (ProjectileSpawnTransformComponent.IsValid() ? 
		ProjectileSpawnTransformComponent->GetForwardVector() : 
		RotationRootComponent->GetForwardVector());

	FTransform SpawnTransform = FTransform(ResultDirection.ToOrientationQuat(), ShootLocation);
	ResultDirection = ResultDirection.RotateAngleAxis(WeaponAsset->FireDirectionRotationOffset.Pitch, SpawnTransform.GetUnitAxis(EAxis::Z));
	ResultDirection = ResultDirection.RotateAngleAxis(WeaponAsset->FireDirectionRotationOffset.Yaw, SpawnTransform.GetUnitAxis(EAxis::X));
	ResultDirection = ResultDirection.RotateAngleAxis(WeaponAsset->FireDirectionRotationOffset.Roll, SpawnTransform.GetUnitAxis(EAxis::Y));

	float DistanceToTarget = (CurrentTarget->GetComponentLocation() - GetActorLocation()).Length();
	float Accuracy = AccuracyOverDistanceCurve.GetRichCurve()->Eval(DistanceToTarget / MaxTargetingDistance);

	WeaponAsset->InaccuracyNoise.ApplyToDirection(ResultDirection, Accuracy, GetGameTimeSinceCreation() + IterationThisTick);

	return ResultDirection;
}
