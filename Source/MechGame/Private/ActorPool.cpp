// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorPool.h"
#include "PooledActor.h"
#include "Engine/World.h"
#include "ActorPoolSubsystem.h"

AActorPool::AActorPool()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AActorPool::InitializePool()
{
	if (PooledActorSubclass == nullptr)
		return;

	UWorld* World = GetWorld();

	if (World == nullptr)
		return;

	UActorPoolSubsystem* ActorPoolSubsystem = World->GetSubsystem<UActorPoolSubsystem>();

	if (ActorPoolSubsystem == nullptr)
		return;

	ActorPoolSubsystem->RegisterActorPool(this);

	PooledActors.Reserve(InitialPoolSize);
	ActiveActors.Reserve(InitialPoolSize);

	PopulatePool(InitialPoolSize);



	bPoolInitialized = true;
}

void AActorPool::BeginPlay()
{
	Super::BeginPlay();
	
	if (bPoolInitialized == false)
		InitializePool();
}

void AActorPool::PopulatePool(int SpawnedActorCount)
{
	if (PooledActorSubclass == nullptr)
		return;

	UWorld* World = GetWorld();

	if (World == nullptr)
		return;

	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.Owner = this;

	for (int32 i = 0; i < SpawnedActorCount; i++)
	{
		APooledActor* NewPooledActor = World->SpawnActor<APooledActor>(PooledActorSubclass, FTransform::Identity, SpawnParams);

		if (NewPooledActor == nullptr)
			break;

		NewPooledActor->RegisterSourcePool(this);
		NewPooledActor->Deactivate();

		PooledActors.Push(NewPooledActor);
	}
}

APooledActor* AActorPool::GetPooledActor(bool ActivatePooledActor)
{
	if (PooledActors.Num() == 0)
		PopulatePool(PoolExpandCount);

	if (PooledActors.Num() > 0)
	{
		APooledActor* PooledActor = PooledActors.Pop(false).Get();
		ActiveActors.Push(PooledActor);

		if (ActivatePooledActor)
			PooledActor->Activate();

		return PooledActor;
	}

	return nullptr;
}

void AActorPool::ReturnActorToPool(APooledActor* PooledActor)
{
	if (PooledActor == nullptr)
		return;

	if (PooledActor->bActive)
		PooledActor->Deactivate();

	if (ActiveActors.Contains(PooledActor))
		ActiveActors.Remove(PooledActor);

	PooledActors.Push(PooledActor);
}