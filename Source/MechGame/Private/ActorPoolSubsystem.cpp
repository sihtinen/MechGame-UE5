// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorPoolSubsystem.h"
#include "ActorPool.h"
#include "PooledActor.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void UActorPoolSubsystem::RegisterActorPool(AActorPool* ActorPool)
{
	if (ActorPool == nullptr)
		return;

	if (RegisteredActorPools.Contains(ActorPool->PooledActorSubclass) == false)
		RegisteredActorPools.Add(ActorPool->PooledActorSubclass, ActorPool);
}

AActorPool* UActorPoolSubsystem::GetActorPool(TSubclassOf<class APooledActor> QueryPooledObjectSubclass)
{
	if (QueryPooledObjectSubclass == nullptr)
		return nullptr;

	// ideally returns matching registered Actor Pool from fast lookup collection

	if (RegisteredActorPools.Contains(QueryPooledObjectSubclass))
		return RegisteredActorPools[QueryPooledObjectSubclass].Get();

	// fallback 1 -> try to find a matching Actor Pool that hasn't been initialized yet

	UWorld* World = GetWorld();

	if (World == nullptr)
		return nullptr;

	UGameplayStatics::GetAllActorsOfClass(this, AActorPool::StaticClass(), PooledActorsSearchResults);

	if (PooledActorsSearchResults.Num() > 0)
	{
		for (int32 i = 0; i < PooledActorsSearchResults.Num(); i++)
		{
			AActorPool* CurrentPool = Cast<AActorPool>(PooledActorsSearchResults[i]);

			if (CurrentPool == nullptr)
				continue;

			if (CurrentPool->PooledActorSubclass == QueryPooledObjectSubclass)
			{
				CurrentPool->InitializePool();
				return CurrentPool;
			}
		}

		PooledActorsSearchResults.Reset();
	}

	// fallback 2 -> create new Actor Pool and assign required Pooled Actor Subclass
	
	AActorPool* NewActorPool = World->SpawnActor<AActorPool>(AActorPool::StaticClass(), FTransform::Identity, FActorSpawnParameters());

	if (NewActorPool == nullptr)
		return nullptr;

	NewActorPool->PooledActorSubclass = QueryPooledObjectSubclass;
	NewActorPool->InitializePool();

	return NewActorPool;
}