// Fill out your copyright notice in the Description page of Project Settings.


#include "ContextSubsystem.h"
#include "ContextTargetComponent.h"
#include "Logging/StructuredLog.h"

void UContextSubsystem::RegisterContextTargetComponent(UContextTargetComponent* Component)
{
	if (Component == nullptr)
	{
		UE_LOGFMT(LogScript, Error, "UContextSubsystem::RegisterContextTargetComponent(): input parameter Component is null pointer");
		return;
	}

	if (ActiveContextTargetComponents.Contains(Component) == false)
	{
		ActiveContextTargetComponents.Add(Component);
	}
	else
	{
		FString ActorName = AActor::GetDebugName(Component->GetOwner());
		UE_LOGFMT(LogScript, Error, "UContextSubsystem::RegisterContextTargetComponent(): ActiveContextTargetComponents already contains component, owner actor: {0}", ActorName);
	}
}

void UContextSubsystem::UnregisterContextTargetComponent(UContextTargetComponent* Component)
{
	if (Component == nullptr)
	{
		UE_LOGFMT(LogScript, Error, "UContextSubsystem::UnregisterContextTargetComponent(): input parameter Component is null pointer");
		return;
	}

	if (ActiveContextTargetComponents.Contains(Component))
	{
		ActiveContextTargetComponents.Remove(Component);
	}
	else
	{
		FString ActorName = AActor::GetDebugName(Component->GetOwner());
		UE_LOGFMT(LogScript, Error, "UContextSubsystem::UnregisterContextTargetComponent(): ActiveContextTargetComponents does not contain component, owner actor: {0}", ActorName);
	}
}

void UContextSubsystem::GetContextTargetsOnLayers(TArray<TWeakObjectPtr<UContextTargetComponent>>* ArrayToPopulate, const int32& QueryLayers)
{
	if (ArrayToPopulate == nullptr)
	{
		UE_LOGFMT(LogScript, Error, "UContextSubsystem::GetAllActiveContextTargetComponents(): input parameters array is nullptr");
		return;
	}

	int32 ActiveTargetsCount = ActiveContextTargetComponents.Num();

	for (int32 i = 0; i < ActiveTargetsCount; i++)
	{
		auto Current = ActiveContextTargetComponents[i];
		
		if (Current == nullptr || Current.IsValid() == false)
			continue;

		auto Component = Current.Get();
		int32 ComponentLayers = Component->ContextLayerFlags;

		for (uint8 ii = 0; ii < 32; ii++)
		{
			if (BitMaskIncludes(QueryLayers, ii) && BitMaskIncludes(ComponentLayers, ii))
			{
				ArrayToPopulate->Add(Component);
				break;
			}
		}
	}
}

bool UContextSubsystem::BitMaskIncludes(const int32& BitMask, const int32& Layer)
{
	return ((BitMask & 1 << Layer) > 0);
}