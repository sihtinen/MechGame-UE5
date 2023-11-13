// Fill out your copyright notice in the Description page of Project Settings.

#include "MechEquipmentComponentRTBase.h"

UMechEquipmentComponentRTBase::UMechEquipmentComponentRTBase()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMechEquipmentComponentRTBase::BeginPlay()
{
	Super::BeginPlay();
}

void UMechEquipmentComponentRTBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FName UMechEquipmentComponentRTBase::GetDisplayName()
{
	return FName();
}

int32 UMechEquipmentComponentRTBase::GetMaxUseCount()
{
	return 0;
}

int32 UMechEquipmentComponentRTBase::GetRemainingUseCount()
{
	return 0;
}
