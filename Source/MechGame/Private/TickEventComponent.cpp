// Fill out your copyright notice in the Description page of Project Settings.


#include "TickEventComponent.h"

UTickEventComponent::UTickEventComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTickEventComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	OnTickEvent.Broadcast(DeltaTime);
}

