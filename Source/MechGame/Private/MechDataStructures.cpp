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
		return ContextTargetComponent->GetComponentVelocity();

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