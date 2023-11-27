// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthUpdated, int32, PreviousHealth, int32, CurrentHealth);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MECHGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentHealth = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxHealth = 1000;

	UPROPERTY(EditAnywhere)
	bool bSelfInitialize = false;

	UPROPERTY(BlueprintAssignable)
	FOnHealthUpdated OnHealthUpdated;

public:	

	UHealthComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	

	void Initialize(int32 MaxHealth);

	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
