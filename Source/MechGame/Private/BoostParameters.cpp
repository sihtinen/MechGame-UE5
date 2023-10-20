// Fill out your copyright notice in the Description page of Project Settings.


#include "BoostParameters.h"

FBoostParameters FBoostParameters::Lerp(const FBoostParameters& a, const FBoostParameters& b, const float& t)
{
	FBoostParameters Result;
	Result.HorizontalForce = FMath::Lerp(a.HorizontalForce, b.HorizontalForce, t);
	Result.VerticalForce = FMath::Lerp(a.VerticalForce, b.VerticalForce, t);
	return Result;
}