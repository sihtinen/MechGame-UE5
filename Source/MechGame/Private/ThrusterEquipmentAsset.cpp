// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrusterEquipmentAsset.h"

FBoostParameters FBoostParameters::Lerp(FBoostParameters a, FBoostParameters b, float t)
{
	FBoostParameters Result;
	Result.HorizontalForce = FMath::Lerp(a.HorizontalForce, b.HorizontalForce, t);
	Result.VerticalForce = FMath::Lerp(a.VerticalForce, b.VerticalForce, t);
	return Result;
}

FBoostParameters UThrusterEquipmentAsset::GetSustainedBoostParams(float inputAmount)
{
	return FBoostParameters::Lerp(SustainedBoostParams_Vertical, SustainedBoostParams_Horizontal, inputAmount);
}

FBoostParameters UThrusterEquipmentAsset::GetDashBoostParams(float inputAmount)
{
	return FBoostParameters::Lerp(DashBoostParams_Vertical, DashBoostParams_Horizontal, inputAmount);
}