// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bSelfInitialize)
		Initialize(MaxHealth);

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
}

void UHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetOwner()->OnTakeAnyDamage.RemoveDynamic(this, &UHealthComponent::TakeDamage);
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::Initialize(int32 NewMaxHealth)
{
	MaxHealth = NewMaxHealth;
	CurrentHealth = NewMaxHealth;

	GetOwner()->SetCanBeDamaged(CurrentHealth > 0);
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	int32 PreviousHealth = CurrentHealth;
	int32 DamageInt = FMath::RoundToInt32(Damage);

	CurrentHealth = FMath::Clamp(CurrentHealth - DamageInt, 0, MaxHealth);

	GetOwner()->SetCanBeDamaged(CurrentHealth > 0);

	OnHealthUpdated.Broadcast(PreviousHealth, CurrentHealth);
}