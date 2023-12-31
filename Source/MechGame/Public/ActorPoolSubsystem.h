// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ActorPoolSubsystem.generated.h"

UCLASS()
class MECHGAME_API UActorPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

	void RegisterActorPool(class AActorPool* ActorPool);

	UFUNCTION(BlueprintCallable)
	AActorPool* GetActorPool(TSubclassOf<class APooledActor> PooledObjectSubclass);

	UFUNCTION(BlueprintCallable)
	class APooledActor* GetPooledActor(TSubclassOf<class APooledActor> PooledObjectSubclass, bool ActivateActor = true);

private:

	UPROPERTY()
	TArray<AActor*> PooledActorsSearchResults;

	UPROPERTY()
	TMap<TSubclassOf<class APooledActor>, TWeakObjectPtr<class AActorPool>> RegisteredActorPools;
};
