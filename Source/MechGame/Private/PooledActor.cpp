// Fill out your copyright notice in the Description page of Project Settings.


#include "PooledActor.h"
#include "ActorPool.h"

APooledActor::APooledActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bActive = false;
}

void APooledActor::RegisterSourcePool(AActorPool* Pool)
{
	SourcePool = Pool;
}

void APooledActor::Activate()
{
	if (bActive)
		return;

	bActive = true;

	OnPooledActorActivated();
}

void APooledActor::Deactivate()
{
	if (bActive == false)
		return;

	bActive = false;

	OnPooledActorDeactivated();
}

void APooledActor::ReturnToPool()
{
	if (SourcePool.IsValid() == false)
		return;

	SourcePool->ReturnActorToPool(this);
}