// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TickEventComponent.h"
#include "Delegates/Delegate.h"
#include "MechPhysicsAsset.h"
#include "PIDAsset.h"
#include "PIDState.h"
#include "Components/CapsuleComponent.h"
#include "PhysicsUtils.h"
#include "Mech.generated.h"

UCLASS()
class MECHGAME_API AMech : public APawn
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mech Base Settings")
	TObjectPtr<UMechPhysicsAsset> PhysicsDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mech Base Settings")
	TObjectPtr<UPIDAsset> RideHeightPID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mech Base Settings")
	TObjectPtr<UPIDAsset> ForwardDirectionPID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mech Runtime Properties")
	bool bBoostInputActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mech Runtime Properties")
	FVector2f MoveInput;

	UPROPERTY(BlueprintReadOnly, Category="Mech Runtime Properties")
	FHitResult GroundHitResult;

	FOnTickEvent OnPrePhysicsTickEvent;
	FOnTickEvent OnPostPhysicsTickEvent;

public:

	AMech();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	FVector GetPivotWorldLocation();

	UFUNCTION(BlueprintCallable)
	FVector GetControlDirection(FVector Direction, bool FlattenToHorizontalPlane);

protected:

	TArray<AActor*> PhysicsTraceIgnoredActors;

	TWeakObjectPtr<UCapsuleComponent> CollisionCapsule;

protected:

	virtual void PreInitializeComponents() override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPrePhysicsTick(float DeltaTime);

	UFUNCTION()
	void OnPostPhysicsTick(float DeltaTime);

	virtual void OnPrePhysicsTick_Implementation(float DeltaTime);

	virtual void OnPostPhysicsTick_Implementation(float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPrePhysicsTick_BP(float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPostPhysicsTick_BP(float DeltaTime);

private:

	TWeakObjectPtr<UTickEventComponent> PrePhysicsTickComponent;
	TWeakObjectPtr<UTickEventComponent> PostPhysicsTickComponent;

	FPIDState RideHeightPIDState;
	FPIDState ForwardDirectionPIDState;

private:

	void DoSurfaceCheck();

	void UpdateRideHeightPID(float DeltaTime);

	void UpdateForwardDirection(float DeltaTime);
};
