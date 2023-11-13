// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TickEventComponent.h"
#include "PIDState.h"
#include "PhysicsPublic.h"
#include "AsyncTickPawn.h"
#include "MechDataStructures.h"
#include "Mech.generated.h"

class UMechPhysicsAsset;
class UMechLoadoutAsset;
class UMechEquipmentAsset;
class UPIDAsset;
class UCapsuleComponent;
class UAnimBlueprint;
class UMechEquipmentComponentRTBase;
class UMechWeaponComponent;
class UMechTargetingComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInputSlotStateUpdated, const EEquipmentSlotType&, SlotType, bool, IsPressed);

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

	UPROPERTY()
	TWeakObjectPtr<UMechTargetingComponent> TargetingComponent;

	UPROPERTY(BlueprintReadWrite, Category = "Mech Runtime Properties")
	bool bBoostInputActive;

	UPROPERTY(BlueprintReadWrite, Category = "Mech Runtime Properties")
	FVector2f MoveInput;

	UPROPERTY(BlueprintReadOnly, Category = "Mech Runtime Properties")
	FHitResult GroundHitResult;

	UPROPERTY(BlueprintReadOnly, Category = "Mech Runtime Properties")
	float SustainedBoostForceVertical;

	UPROPERTY(BlueprintReadOnly, Category = "Mech Runtime Properties")
	float SustainedBoostForceHorizontal;

	UPROPERTY(BlueprintReadOnly, Category = "Mech Runtime Properties")
	FVector DashBoostForce;

	UPROPERTY(BlueprintReadOnly, Category = "Mech Runtime Properties")
	float BodyRotationAngle;

	UPROPERTY(EditAnywhere, Category = "Mech Runtime Properties")
	float BodyRotationTargetValueRate;

	UPROPERTY(EditAnywhere, Category = "Mech Runtime Properties")
	float BodyRotationSmoothingTime;

	UPROPERTY(EditAnywhere, Category = "Mech Runtime Properties")
	float BodyRotationDampingRatio;

	UPROPERTY(BlueprintAssignable)
	FOnInputSlotStateUpdated OnInputSlotStateUpdated;

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

	UFUNCTION(BlueprintCallable)
	void SetAnimationBlueprint(USkeletalMeshComponent* SkeletalMeshComponent, TSubclassOf<UAnimInstance> AnimBlueprint);

	UFUNCTION(BlueprintCallable)
	bool IsGrounded();

	UFUNCTION(BlueprintCallable)
	void SetFixedFrameRate(uint8 FrameRateTarget);

	void RegisterWeaponComponent(UMechWeaponComponent* Component, const EEquipmentSlotType& SlotType);

	UFUNCTION(BlueprintCallable)
	UMechEquipmentComponentRTBase* GetEquipmentRuntimeComponent(const EEquipmentSlotType& SlotType);

	UFUNCTION(BlueprintCallable)
	UMechWeaponComponent* GetWeaponComponent(const EEquipmentSlotType& SlotType);

	UFUNCTION(BlueprintCallable)
	void UpdateSlotInputState(const EEquipmentSlotType& SlotInput, const bool& IsPressed);

	UFUNCTION(BlueprintCallable)
	bool IsPlayerPawn();

protected:

	UPROPERTY()
	TArray<AActor*> PhysicsTraceIgnoredActors;

	UPROPERTY()
	TWeakObjectPtr<UCapsuleComponent> CollisionCapsule;

	UPROPERTY()
	TMap<EEquipmentSlotType, TWeakObjectPtr<UMechEquipmentComponentRTBase>> RuntimeEquipmentComponentsMap;

	UPROPERTY()
	TMap<EEquipmentSlotType, bool> SlotInputStates;

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnLoadoutAssetsLoadedToMemory();

private:

	float BodyRotationAngle_ValueRate;

	FPIDState RideHeightPIDState;
	FPIDState ForwardDirectionPIDState;

	TSet<int32> PreventMovementSources;

private:

	void StartLoadingLoadoutAssets();

	void Internal_OnLoadoutAssetsLoadedToMemory();

	void DoSurfaceCheck(bool IsAsync);

	void UpdateRideHeightPID(float DeltaTime);

	void UpdateForwardDirection(float DeltaTime);

	void UpdateMovement(float DeltaTime);

	void UpdateDrag(float DeltaTime);

	void UpdateBodyRotation(float DeltaTime);
};
