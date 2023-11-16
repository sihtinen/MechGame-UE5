// Fill out your copyright notice in the Description page of Project Settings.


#include "MechDataStructures.h"
#include "ProjectileAsset.h"
#include "ContextTargetComponent.h"

FTargetingOption::FTargetingOption()
{
	IsValid = false;
	DotScore = 0;
	DistanceScore = 0;
	TotalScore = 0;
}

FVector FTargetingOption::GetLocation()
{
	if (ContextTargetComponent.IsValid())
		return ContextTargetComponent->GetComponentLocation();

	return FVector();
}

FVector FTargetingOption::GetVelocity()
{
	if (ContextTargetComponent.IsValid())
		return ContextTargetComponent->GetCalculatedVelocity();

	return FVector();
}

FProjectileState::FProjectileState()
{
	AliveTime = 0.0f;
}

FProjectileState::FProjectileState(AActor* Owner, UProjectileAsset* Asset)
{
	OwnerActor = Owner;
	ProjectileAsset = Asset;

	AliveTime = 0.0f;
}

void FWeaponInaccuracyNoise::ApplyToDirection(FVector& Direction, const float Accuracy, const float GameTime)
{
	float Inaccuracy = 1.0 - Accuracy;

	FRotator Rotator = FRotator(
		Inaccuracy * FMath::PerlinNoise1D(GameTime * NoiseSpeed.X) * NoiseScale.X,
		Inaccuracy * FMath::PerlinNoise1D(GameTime * NoiseSpeed.Y) * NoiseScale.Y,
		Inaccuracy * FMath::PerlinNoise1D(GameTime * NoiseSpeed.Z) * NoiseScale.Z);

	Direction = Rotator.RotateVector(Direction);
}

void FAccuracyConfig::UpdateTick(float& Accuracy, const float& DeltaTime, const float& VelocityMagnitude)
{
	if (FMath::IsNearlyZero(MaxVelocity) == false)
	{
		float VelocityNormalized = FMath::Clamp(VelocityMagnitude / MaxVelocity, 0.0f, 1.0f);
		float VelocityLossRate = LossOverVelocityRate * LossOverMaxVelocityCurve.GetRichCurve()->Eval(VelocityNormalized);
		Accuracy = FMath::Clamp(Accuracy - DeltaTime * VelocityLossRate, 0.0f, 1.0f);
	}

	float RegainRate = BaseRegainSpeed * RegainRateCurve.GetRichCurve()->Eval(Accuracy);
	Accuracy = FMath::Clamp(Accuracy + DeltaTime * RegainRate, 0.0f, 1.0f);
}

void FAccuracyConfig::ApplyUsageLoss(float& Accuracy)
{
	Accuracy = FMath::Clamp(Accuracy - LossPerUse, 0.0f, 1.0f);
}