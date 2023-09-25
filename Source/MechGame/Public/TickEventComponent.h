// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Delegates/Delegate.h"
#include "TickEventComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTickEvent, float, DeltaTime);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MECHGAME_API UTickEventComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTickEventComponent();

	FOnTickEvent OnTickEvent;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;		
};
