// Fill out your copyright notice in the Description page of Project Settings.


#include "MechDataStructures.h"

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