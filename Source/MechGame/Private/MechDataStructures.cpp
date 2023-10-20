// Fill out your copyright notice in the Description page of Project Settings.


#include "MechDataStructures.h"
#include "ProjectileAsset.h"

FTargetingOption::FTargetingOption()
{
	IsValid = false;
	DotScore = 0;
	DistanceScore = 0;
	TotalScore = 0;
}

FVector FTargetingOption::GetLocation()
{
	return FVector();
}

FVector FTargetingOption::GetVelocity()
{
	return FVector();
}

FProjectileState::FProjectileState()
{
}

FProjectileState::FProjectileState(AActor* Owner, UProjectileAsset* Asset)
{
	OwnerActor = Owner;
	ProjectileAsset = Asset;

	AliveTime = 0.0f;
}