// Fill out your copyright notice in the Description page of Project Settings.


#include "MechWeaponComponent.h"
#include "Mech.h"
#include "MechProjectileWeaponAsset.h"
#include "ProjectileAsset.h"
#include "MechTargetingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ContextTargetComponent.h"
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

	bool bShotPendingThisTick = (NumPendingShots > 0) || (bInputActive && RemainingUseCount > 0);

	if (bShotPendingThisTick == false)
		return;

	const double TimeNow = UGameplayStatics::GetTimeSeconds(this);
	const double WaitTimeBetweenShots = 1.0 / SettingsAsset->UseRatePerSecond;
	double TimeElapsedPreviouslyFired = TimeNow - TimePreviouslyFired;

	if (SettingsAsset->bBurstFire)
	{
		if (NumPendingShots > 0)
		{
			if (TimeElapsedPreviouslyFired < WaitTimeBetweenShots)
				return;
		}
		else
		{
			if (TimeElapsedPreviouslyFired < SettingsAsset->BurstCooldownTime)
				return;

			NumPendingShots = SettingsAsset->BurstNumShots;

			if (NumPendingShots <= 0)
				NumPendingShots = 1;

			TimeElapsedPreviouslyFired = WaitTimeBetweenShots;
		}
	}
	else
	{
		if (TimeElapsedPreviouslyFired < WaitTimeBetweenShots)
			return;

		NumPendingShots = 1;
	}

	bFiredPreviousTick = true;
	TimePreviouslyFired = TimeNow;

	Shoot(TimeElapsedPreviouslyFired, WaitTimeBetweenShots);
}

void UMechWeaponComponent::Shoot(const double& TimeElapsedPreviouslyFired, const double& WaitTimeBetweenShots)
{
	UWorld* World = GetWorld();

	if (World == nullptr)
	{
		NumPendingShots = 0;
		return;
	}

	UProjectileSubsystem* ProjectileSubsystem = World->GetSubsystem<UProjectileSubsystem>();

	if (ProjectileSubsystem == nullptr)
	{
		NumPendingShots = 0;
		return;
	}

	FVector ProjectileSpawnLocation = GetOwner()->GetActorLocation();
	FVector ShootDirection = GetShootDirection(ProjectileSpawnLocation);
	float RemainingTime = TimeElapsedPreviouslyFired;
	int32 NumSpawned = 0;
	int32 PreviousUseCount = RemainingUseCount;

	FTargetingOption BestTarget = Mech->TargetingComponent->GetBestTargetingOption();
	UContextTargetComponent* TargetComponent = (BestTarget.IsValid ? BestTarget.ContextTargetComponent.Get() : nullptr);

	while (RemainingTime >= WaitTimeBetweenShots)
	{
		RemainingTime -= WaitTimeBetweenShots;

		if (NumPendingShots <= 0 || RemainingUseCount <= 0)
		{
			NumPendingShots = 0;
			break;
		}

		FVector InstanceSpawnLocation = ProjectileSpawnLocation;

		if (NumSpawned > 0)
			InstanceSpawnLocation += (NumSpawned * WaitTimeBetweenShots) * SettingsAsset->ProjectileAsset->InitialSpeed * ShootDirection;

		ProjectileSubsystem->SpawnProjectile(
			Mech.Get(), 
			SettingsAsset->ProjectileAsset, 
			InstanceSpawnLocation, 
			ShootDirection, 
			TargetComponent);

		RemainingUseCount--;
		NumPendingShots--;
		NumSpawned++;
	}

	OnUseCountUpdated.Broadcast(RemainingUseCount, PreviousUseCount);
}

bool UMechWeaponComponent::IsAiming()
{
	return bInputActive || (NumPendingShots > 0);
}

FName UMechWeaponComponent::GetDisplayName()
{
	if (SettingsAsset.IsValid())
		return SettingsAsset->DisplayName;

	return FName();
}

int32 UMechWeaponComponent::GetMaxUseCount()
{
	if (SettingsAsset.IsValid())
		return SettingsAsset->UseCount;

	return 0;
}

int32 UMechWeaponComponent::GetRemainingUseCount()
{
	return RemainingUseCount;
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