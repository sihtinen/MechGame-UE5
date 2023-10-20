// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorPoolSubsystem.h"
#include "ActorPool.h"
#include "PooledActor.h"
#include "Engine/World.h"

void UActorPoolSubsystem::RegisterActorPool(AActorPool* ActorPool)
{
	if (ActorPool == nullptr)
		return;

	if (RegisteredActorPools.Contains(ActorPool->PooledActorSubclass) == false)
		RegisteredActorPools.Add(ActorPool->PooledActorSubclass, ActorPool);
}

AActorPool* UActorPoolSubsystem::GetActorPool(TSubclassOf<class APooledActor> PooledObjectSubclass)
{
	if (PooledObjectSubclass == nullptr)
		return nullptr;

	if (RegisteredActorPools.Contains(PooledObjectSubclass))
		return RegisteredActorPools[PooledObjectSubclass].Get();

	UWorld* World = GetWorld();

	if (World == nullptr)
		return nullptr;

	AActorPool* ActorPool = World->SpawnActor<AActorPool>(AActorPool::StaticClass(), FTransform::Identity, FActorSpawnParameters());

	if (ActorPool == nullptr)
		return nullptr;

	ActorPool->PooledActorSubclass = PooledObjectSubclass;
	ActorPool->InitializePool();

	return ActorPool;
}