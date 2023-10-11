// Fill out your copyright notice in the Description page of Project Settings.


#include "MechWeaponComponent.h"

UMechWeaponComponent::UMechWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMechWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMechWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UMechWeaponComponent::IsAiming()
{
	return true;
}