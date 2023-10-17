// Fill out your copyright notice in the Description page of Project Settings.

#include "MechTargetingComponent.h"
#include "ContextSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "ContextTargetComponent.h"
#include "Logging/StructuredLog.h"

UMechTargetingComponent::UMechTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMechTargetingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMechTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ValidContextTargetsArray.Reset();
	ValidTargetingOptions.Reset();

	CalculateValidContextTargets();

	// filter results further in Blueprint / other C++ code if needed
	OnValidTargetingOptionsCollected.Broadcast();


}

void UMechTargetingComponent::CalculateValidContextTargets()
{
	auto GameInstance = UGameplayStatics::GetGameInstance(GetOwner());

	if (GameInstance == nullptr)
		return;

	auto ContextSubsystem = GameInstance->GetSubsystem<UContextSubsystem>();

	if (ContextSubsystem == nullptr)
		return;

	ContextSubsystem->GetContextTargetsOnLayers(&ValidContextTargetsArray, TargetContextLayerFlags);

	uint32 SelfOwnerID = GetOwner()->GetUniqueID();

	for (int32 i = ValidContextTargetsArray.Num(); i-- > 0;)
	{
		auto Current = ValidContextTargetsArray[i];
		auto CurrentComponent = Current.Get();

		uint32 ComponentOwnerID = CurrentComponent->GetOwner()->GetUniqueID();

		if (ComponentOwnerID == SelfOwnerID)
		{
			ValidContextTargetsArray.RemoveAt(i);
			continue;
		}

		auto NewTargetingOption = FTargetingOption();
		NewTargetingOption.ContextTargetComponent = CurrentComponent;

		ValidTargetingOptions.Add(NewTargetingOption);
	}
}