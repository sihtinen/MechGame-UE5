// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorPool.generated.h"

UCLASS()
class MECHGAME_API AActorPool : public AActor
{
	GENERATED_BODY()
	
public:

	bool bPoolInitialized = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class APooledActor> PooledActorSubclass;

	UPROPERTY(EditAnywhere)
	int InitialPoolSize = 32;

	UPROPERTY(EditAnywhere)
	int PoolExpandCount = 16;

public:

	AActorPool();

	void InitializePool();

	UFUNCTION(BlueprintCallable)
	APooledActor* GetPooledActor(bool ActivatePooledActor = true);

	UFUNCTION(BlueprintCallable)
	void ReturnActorToPool(APooledActor* PooledActor);

protected:

	virtual void BeginPlay() override;

private:

	TQueue<TWeakObjectPtr<APooledActor>> PooledActors;
	TArray<TWeakObjectPtr<APooledActor>> ActiveActors;

private:

	void PopulatePool(int SpawnedActorCount);
};
