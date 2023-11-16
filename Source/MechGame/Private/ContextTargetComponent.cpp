// Fill out your copyright notice in the Description page of Project Settings.

#include "ContextTargetComponent.h"
#include "ContextSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"

UContextTargetComponent::UContextTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UContextTargetComponent::InitializeComponent()
{
	Super::InitializeComponent();

	auto GameInstance = UGameplayStatics::GetGameInstance(GetOwner());
	if (GameInstance)
	{
		auto ContextSubsystem = GameInstance->GetSubsystem<UContextSubsystem>();
		if (ContextSubsystem)
		{
			ContextSubsystem->RegisterContextTargetComponent(this);
		}
	}

	LocationPrevTick = GetComponentLocation();
}

void UContextTargetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	auto GameInstance = UGameplayStatics::GetGameInstance(GetOwner());
	if (GameInstance)
	{
		auto ContextSubsystem = GameInstance->GetSubsystem<UContextSubsystem>();
		if (ContextSubsystem)
		{
			ContextSubsystem->UnregisterContextTargetComponent(this);
		}
	}
}

void UContextTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector CurrentLocation = GetComponentLocation();

	CalculatedVelocity = (CurrentLocation - LocationPrevTick) / DeltaTime;

	LocationPrevTick = CurrentLocation;
}