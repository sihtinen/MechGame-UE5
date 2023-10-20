// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PooledActor.generated.h"

UCLASS()
class MECHGAME_API APooledActor : public AActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly)
	bool bActive;

public:

	APooledActor();

	void RegisterSourcePool(class AActorPool* SourcePool);

	UFUNCTION(BlueprintCallable)
	void Activate();

	UFUNCTION(BlueprintCallable)
	void Deactivate();

	UFUNCTION(BlueprintCallable)
	void ReturnToPool();

protected:

	UFUNCTION(BlueprintImplementableEvent)
	void OnPooledActorActivated();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPooledActorDeactivated();

private:

	TWeakObjectPtr<class AActorPool> SourcePool;
};
