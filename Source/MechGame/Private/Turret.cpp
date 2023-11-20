// Fill out your copyright notice in the Description page of Project Settings.

#include "Turret.h"
#include "ContextSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "ContextTargetComponent.h"

ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SelectTargetRemainingWaitTime = 0.0f;
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
	DefaultRotator = GetActorRotation();

	SelectTargetRemainingWaitTime = FMath::RandRange(0.0f, SelectTargetUpdateRate);

	TraceIgnoredActors.Add(GetOwner());
	TraceIgnoredActors.Add(nullptr);
}

void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WeaponAsset == false)
		return;

	SelectTargetRemainingWaitTime -= DeltaTime;

	if (SelectTargetRemainingWaitTime <= 0)
	{
		SelectTargetRemainingWaitTime += SelectTargetUpdateRate;
		SelectTarget();
	}

	// turret rotation

	// fire logic
}

void ATurret::SelectTarget()
{
	auto GameInstance = UGameplayStatics::GetGameInstance(GetOwner());

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
			continue;

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
		return false;

	UContextTargetComponent* TargetPtr = Target.Get();

	FVector ToTargetVector = TargetPtr->GetComponentLocation() - GetActorLocation();

	if (ToTargetVector.SquaredLength() > (MaxTargetingDistance * MaxTargetingDistance))
		return false;

	if (bLimitRotation)
	{
		FRotator MyRotator = GetActorRotation().GetNormalized();
		FRotator ToTargetRotator = ToTargetVector.GetSafeNormal().ToOrientationRotator();

		if (FMath::Abs(ToTargetRotator.Pitch - MyRotator.Pitch) > RotationLimitPitch ||
			FMath::Abs(ToTargetRotator.Yaw - MyRotator.Yaw) > RotationLimitYaw)
			return false;
	}

	TraceIgnoredActors[1] = TargetPtr->GetOwner();

	FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams();
	CollisionQueryParams.AddIgnoredActors(TraceIgnoredActors);

	bool bObstructionFound = GetWorld()->LineTraceTestByChannel(
		GetActorLocation(),
		TargetPtr->GetComponentLocation(),
		ECollisionChannel::ECC_PhysicsBody,
		CollisionQueryParams);

	TraceIgnoredActors[1] = nullptr;

	if (bObstructionFound)
		return false;

	return true;
}

float ATurret::CalculateTargetScore(const TWeakObjectPtr<UContextTargetComponent>& Target)
{
	return 0.0f;
}
