// Fill out your copyright notice in the Description page of Project Settings.


#include "MechWeaponComponent.h"
#include "Mech.h"
#include "MechProjectileWeaponAsset.h"
#include "ProjectileAsset.h"
#include "MechTargetingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectileSubsystem.h"

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

	RemainingUseCount = SettingsAsset->UseCount;

	Mech->RegisterWeaponComponent(this, SlotType);
	Mech->OnInputSlotStateUpdated.AddDynamic(this, &UMechWeaponComponent::OnInputSlotStateUpdated);
}

void UMechWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Mech.IsValid() == false || SettingsAsset.IsValid() == false)
		return;

	bool _FiredLastTick = bFiredPreviousTick;
	bFiredPreviousTick = false;

	if (bInputActive == false || RemainingUseCount <= 0)
		return;

	double TimeNow = UGameplayStatics::GetTimeSeconds(this);
	double TimeSincePreviouslyFired = TimeNow - TimePreviouslyFired;
	double WaitTimeBetweenShots = 1.0 / SettingsAsset->UseRatePerSecond;

	if (TimeSincePreviouslyFired < WaitTimeBetweenShots)
		return;

	bFiredPreviousTick = true;
	TimePreviouslyFired = TimeNow;

	UWorld* World = GetWorld();

	if (World == nullptr)
		return;

	UProjectileSubsystem* ProjectileSubsystem = World->GetSubsystem<UProjectileSubsystem>();

	if (ProjectileSubsystem == nullptr)
		return;

	FVector ProjectileSpawnLocation = GetOwner()->GetActorLocation();
	FVector ShootDirection = GetShootDirection(ProjectileSpawnLocation);

	int NewProjectilesCount = (_FiredLastTick ? FMath::Floor(1 + DeltaTime / WaitTimeBetweenShots) : 1);

	for (int32 i = 0; i < NewProjectilesCount; i++)
	{
		FVector InstanceSpawnLocation = ProjectileSpawnLocation;

		if (i > 0)
			InstanceSpawnLocation += DeltaTime * SettingsAsset->ProjectileAsset->InitialSpeed * ShootDirection;

		ProjectileSubsystem->SpawnProjectile(Mech.Get(), SettingsAsset->ProjectileAsset, InstanceSpawnLocation, ShootDirection);

		RemainingUseCount--;

		Execute_OnUseCountUpdated(this, RemainingUseCount);
	}
}

bool UMechWeaponComponent::IsAiming()
{
	return bInputActive;
}

void UMechWeaponComponent::OnInputSlotStateUpdated(const EEquipmentSlotType& SlotType, bool IsPressed)
{
	if (SlotType != Slot)
		return;

	bInputActive = IsPressed;
}

FVector UMechWeaponComponent::GetShootDirection(const FVector& ShootLocation)
{
	if (Mech->TargetingComponent->ValidTargetingOptions.Num() > 0)
	{
		FTargetingOption BestTarget = Mech->TargetingComponent->GetBestTargetingOption();
		FVector ToTarget = (BestTarget.GetLocation() - ShootLocation);
		return (ToTarget.GetUnsafeNormal());
	}

	if (Mech->IsPlayerControlled())
	{
		const FVector& WorldTargetLocation = Mech->TargetingComponent->WorldTargetLocation;
		FVector ToWorldTarget = (WorldTargetLocation - ShootLocation);
		return (ToWorldTarget.GetUnsafeNormal());
	}

	return Mech->GetActorForwardVector();
}

const FName UMechWeaponComponent::GetDisplayName_Implementation()
{
	if (SettingsAsset.IsValid() == false)
		return GetFName();

	return SettingsAsset->DisplayName;
}

int32 UMechWeaponComponent::GetMaxUseCount_Implementation()
{
	if (SettingsAsset.IsValid() == false)
		return 0;

	return SettingsAsset->UseCount;
}

int32 UMechWeaponComponent::GetRemainingUseCount_Implementation()
{
	return RemainingUseCount;
}
