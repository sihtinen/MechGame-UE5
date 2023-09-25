// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrusterEquipmentAsset.h"

FBoostParameters FBoostParameters::Lerp(FBoostParameters a, FBoostParameters b, float t)
{
	return FBoostParameters();
}

FBoostParameters UThrusterEquipmentAsset::GetSustainedBoostParams(float inputAmount)
{
	return FBoostParameters::Lerp(SustainedBoostParams_Horizontal, SustainedBoostParams_Vertical, inputAmount);
}

FBoostParameters UThrusterEquipmentAsset::GetDashBoostParams(float inputAmount)
{
	return FBoostParameters::Lerp(DashBoostParams_Horizontal, DashBoostParams_Vertical, inputAmount);
}