// Fill out your copyright notice in the Description page of Project Settings.

#include "MechTargetingComponent.h"

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
}