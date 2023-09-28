// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BoostParameters.h"
#include "MechThrusterComponent.generated.h"

class AMech;
class UThrusterEquipmentAsset;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MECHGAME_API UMechThrusterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Thruster Settings")
	TObjectPtr<UThrusterEquipmentAsset> ThrusterAsset;

public:
	UMechThrusterComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<AMech> Mech;
	bool bWasBoostInputActivePreviousFrame;
	bool bDashActive;
	bool bDashInitialized;
	float DashPrepareTimer;
	float DashBoostTimer;
	FVector DashBoostDirection;
	FBoostParameters DashBoostParams;

private:
	void OnBoostInputStarted();
	void OnBoostInputCanceled();
	void UpdateDash(float DeltaTime);
};
