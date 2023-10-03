// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TickEventComponent.h"
#include "PIDState.h"
#include "PhysicsPublic.h"
#include "AsyncTickPawn.h"
#include "Mech.generated.h"

class UMechPhysicsAsset;
class UMechLoadoutAsset;
class UPIDAsset;
class UCapsuleComponent;

UCLASS()
class MECHGAME_API AMech : public AAsyncTickPawn
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mech Base Settings")
	TObjectPtr<UMechPhysicsAsset> PhysicsDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mech Base Settings")
	TObjectPtr<UMechLoadoutAsset> LoadoutAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mech Base Settings")
	TObjectPtr<UPIDAsset> RideHeightPID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mech Base Settings")
	TObjectPtr<UPIDAsset> ForwardDirectionPID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mech Runtime Properties")
	bool bBoostInputActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mech Runtime Properties")
	FVector2f MoveInput;

	UPROPERTY(BlueprintReadOnly, Category = "Mech Runtime Properties")
	FHitResult GroundHitResult;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mech Runtime Properties")
	float SustainedBoostForceVertical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mech Runtime Properties")
	float SustainedBoostForceHorizontal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mech Runtime Properties")
	FVector DashBoostForce;

public:

	AMech();

	virtual void Tick(float DeltaTime) override;

	virtual void NativeAsyncTick(float DeltaTime);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	FVector GetPivotWorldLocation(bool IsAsync = false);

	UFUNCTION(BlueprintCallable)
	FVector GetControlDirection(FVector Direction, bool FlattenToHorizontalPlane);

	UFUNCTION(BlueprintCallable)
	FVector GetWorldInputVectorProjectedToSurface();

	void RegisterPreventMovementSource(int32 sourceID);

	void UnregisterPreventMovementSource(int32 sourceID);


protected:

	UPROPERTY()
	TArray<AActor*> PhysicsTraceIgnoredActors;

	UPROPERTY()
	TWeakObjectPtr<UCapsuleComponent> CollisionCapsule;

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnLoadoutAssetsLoadedToMemory();

private:

	FPIDState RideHeightPIDState;
	FPIDState ForwardDirectionPIDState;

	TSet<int32> PreventMovementSources;

private:

	void StartLoadingLoadoutAssets();

	void DoSurfaceCheck(bool IsAsync);

	void UpdateRideHeightPID(float DeltaTime);

	void UpdateForwardDirection(float DeltaTime);

	void UpdateMovement(float DeltaTime);

	void UpdateDrag(float DeltaTime);
};
