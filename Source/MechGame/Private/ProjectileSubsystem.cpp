// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileSubsystem.h"
#include "Logging/StructuredLog.h"

void UProjectileSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ActiveProjectiles.Reserve(1024);
}

void UProjectileSubsystem::Tick(float DeltaTime)
{

}