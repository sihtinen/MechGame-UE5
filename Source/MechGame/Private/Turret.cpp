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
			TargetQuaternion = InterceptDirection.ToOrientationQuat();
	}

	FQuat NewQuaternion = UKismetMathLibrary::QuaternionSpringInterp(
		RotationRootComponent->GetComponentQuat(),
		TargetQuaternion,
		RotationSpringState,
		RotationSpringStiffness,
		RotationSpringCriticalDamping,
		DeltaTime);

	RotationRootComponent->SetWorldRotation(NewQuaternion);
}