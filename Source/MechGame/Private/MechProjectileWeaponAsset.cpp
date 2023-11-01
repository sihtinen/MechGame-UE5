// Fill out your copyright notice in the Description page of Project Settings.

#include "MechProjectileWeaponAsset.h"
#include "Mech.h"
#include "MechWeaponComponent.h"

void UMechProjectileWeaponAsset::SetupMechRuntime(AMech* Mech, const EEquipmentSlotType& Slot)
{
	bool bManualAttachment = false;

	UMechWeaponComponent* WeaponComponent = Cast<UMechWeaponComponent>(Mech->AddComponentByClass(UMechWeaponComponent::StaticClass(), bManualAttachment, FTransform::Identity, false));

	WeaponComponent->SetupGameplay(Mech, this, Slot);

	Mech->FinishAddComponent(WeaponComponent, bManualAttachment, FTransform::Identity);
	Mech->AddInstanceComponent(WeaponComponent);
}