// Fill out your copyright notice in the Description page of Project Settings.


#include "MechWeaponComponent.h"
#include "Mech.h"
#include "MechProjectileWeaponAsset.h"

UMechWeaponComponent::UMechWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMechWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMechWeaponComponent::SetupGameplay(AMech* MechActor, UMechProjectileWeaponAsset* WeaponAsset, const EEquipmentSlotType& SlotType)
{
	Mech = MechActor;
	SettingsAsset = WeaponAsset;
	Slot = SlotType;

	Mech->RegisterWeaponComponent(this, SlotType);
}

void UMechWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UMechWeaponComponent::IsAiming()
{
	return true;
}