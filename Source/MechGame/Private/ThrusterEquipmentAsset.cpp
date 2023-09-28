// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrusterEquipmentAsset.h"

FBoostParameters UThrusterEquipmentAsset::GetSustainedBoostParams(float inputAmount)
{
	return FBoostParameters::Lerp(SustainedBoostParams_Vertical, SustainedBoostParams_Horizontal, inputAmount);
}

FBoostParameters UThrusterEquipmentAsset::GetDashBoostParams(float inputAmount)
{
	return FBoostParameters::Lerp(DashBoostParams_Vertical, DashBoostParams_Horizontal, inputAmount);
}