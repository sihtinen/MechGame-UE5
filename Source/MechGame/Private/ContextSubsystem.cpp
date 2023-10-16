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