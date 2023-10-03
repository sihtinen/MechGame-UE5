// Fill out your copyright notice in the Description page of Project Settings.


#include "Mech.h"
#include "MechPhysicsAsset.h"
#include "MechLoadoutAsset.h"
#include "MechEquipmentAsset.h"
#include "PIDAsset.h"
#include "PIDState.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PhysicsUtils.h"
#include "AsyncTickFunctions.h"
#include "Engine/AssetManager.h"

AMech::AMech()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMech::BeginPlay()
{
	Super::BeginPlay();
	
	PhysicsTraceIgnoredActors.Add(this);

	CollisionCapsule = Cast<UCapsuleComponent>(GetComponentByClass(UCapsuleComponent::StaticClass()));
	CollisionCapsule->SetLinearDamping(0.0f);

	if (LoadoutAsset)
		StartLoadingLoadoutAssets();
}

void AMech::StartLoadingLoadoutAssets()
{
	TArray<FSoftObjectPath> LoadoutAssetSoftObjectPaths = LoadoutAsset->GetValidAssetSoftObjectPaths();

	if (LoadoutAssetSoftObjectPaths.IsEmpty())
		return;

	FSimpleDelegate Delegate = FStreamableDelegate::CreateUObject(this, &AMech::OnLoadoutAssetsLoadedToMemory);
	UAssetManager::GetStreamableManager().RequestAsyncLoad(LoadoutAssetSoftObjectPaths, Delegate);
}

void AMech::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DoSurfaceCheck(false);

	if (GEngine)
	{
		int32 MyID = GetUniqueID();
		float DT = DeltaTime;
		GEngine->AddOnScreenDebugMessage(MyID + 1, DT, FColor::Green, FString::Printf(TEXT("Delta Time Tick: %f"), DT), false);
	}
}

void AMech::NativeAsyncTick(float DeltaTime)
{
	Super::NativeAsyncTick(DeltaTime);

	if (PhysicsDataAsset == false)
		return;

	DoSurfaceCheck(true);
	UpdateForwardDirection(DeltaTime);
	UpdateDrag(DeltaTime);
	UpdateRideHeightPID(DeltaTime);
	UpdateMovement(DeltaTime);

	if (GEngine)
	{
		int32 MyID = GetUniqueID();
		float DT = DeltaTime;
		GEngine->AddOnScreenDebugMessage(MyID, DT, FColor::Green, FString::Printf(TEXT("Delta Time Async: %f"), DT), false);
	}
}

void AMech::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMech::DoSurfaceCheck(bool IsAsync)
{
	FVector PivotLocation = GetPivotWorldLocation(IsAsync);
	FVector UpDirection = GetTransform().GetUnitAxis(EAxis::Z);
	FVector TraceStart = PivotLocation + PhysicsDataAsset->SurfaceTraceHeight * UpDirection;
	FVector TraceEnd = TraceStart - PhysicsDataAsset->SurfaceTraceDistance * UpDirection;
	ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_PhysicsBody);
	bool TraceAgainstComplexGeo = false;
	bool IgnoreSelf = true;
	EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::ForOneFrame;

	if (IsAsync)
	{
		DrawDebugType = EDrawDebugTrace::None;
	}

	UKismetSystemLibrary::CapsuleTraceSingle(
		this,
		TraceStart,
		TraceEnd,
		PhysicsDataAsset->CollisionCapsuleRadius,
		PhysicsDataAsset->CollisionCapsuleHalfHeight,
		TraceChannel,
		TraceAgainstComplexGeo,
		PhysicsTraceIgnoredActors,
		DrawDebugType,
		GroundHitResult,
		IgnoreSelf);
}

void AMech::UpdateRideHeightPID(float DeltaTime)
{
	if (RideHeightPID == false || CollisionCapsule.IsValid() == false)
		return;

	if (GroundHitResult.bBlockingHit == false && GroundHitResult.bStartPenetrating == false)
	{
		RideHeightPIDState.ResetState();
		return;
	}

	bool IsAsync = true;
	FVector PivotLocation = GetPivotWorldLocation(IsAsync);
	FVector UpDirection = GetTransform().GetUnitAxis(EAxis::Z);
	FVector HitLocation = GroundHitResult.bStartPenetrating ? (PivotLocation + GroundHitResult.PenetrationDepth * UpDirection) : GroundHitResult.ImpactPoint;

	float CurrentValue = FVector::Distance(HitLocation, PivotLocation);
	float TargetValue = PhysicsDataAsset->SurfaceRideHeight;

	RideHeightPIDState = RideHeightPID->UpdateTick(RideHeightPIDState, DeltaTime, CurrentValue, TargetValue);

	float FilteredPIDOutput = RideHeightPIDState.Output;
	if (bBoostInputActive)
		FilteredPIDOutput = FMath::Max(FilteredPIDOutput, 0);

	FVector Force = FilteredPIDOutput * UpDirection;

	UAsyncTickFunctions::ATP_AddForce(CollisionCapsule.Get(), Force, true, NAME_None);
	//CollisionCapsule->AddForce(Force, NAME_None, true);
}

void AMech::UpdateForwardDirection(float DeltaTime)
{
	FTransform Transform = UAsyncTickFunctions::ATP_GetTransform(CollisionCapsule.Get());

	float CurrentYaw = Transform.Rotator().GetNormalized().GetComponentForAxis(EAxis::Z);
	float ControlRotationYaw = GetControlRotation().GetNormalized().GetComponentForAxis(EAxis::Z);
	FVector RotateAxis = Transform.GetUnitAxis(EAxis::Z);

	ForwardDirectionPIDState = ForwardDirectionPID->UpdateAngleTick(ForwardDirectionPIDState, DeltaTime, CurrentYaw, ControlRotationYaw);

	FVector Torque = ForwardDirectionPIDState.Output * RotateAxis;

	UAsyncTickFunctions::ATP_AddTorque(CollisionCapsule.Get(), Torque / DeltaTime, true, NAME_None);

	//ForwardDirectionPIDState = PhysicsUtils::ApplyAngleTorquePID(
	//	ForwardDirectionPID,
	//	CollisionCapsule.Get(),
	//	ForwardDirectionPIDState,
	//	RotateAxis,
	//	DeltaTime,
	//	CurrentYaw,
	//	ControlRotationYaw);
}

void AMech::UpdateMovement(float DeltaTime)
{
	FVector MovementForce = DashBoostForce + SustainedBoostForceVertical * FVector::UpVector;

	if (PreventMovementSources.Num() == 0 && MoveInput.SquaredLength() > FLT_EPSILON)
	{
		bool IsGrounded = (GroundHitResult.bBlockingHit || GroundHitResult.bStartPenetrating);
		float MovementSpeed = (IsGrounded ? PhysicsDataAsset->MovementSpeedGround : PhysicsDataAsset->MovementSpeedAir);
		FVector WorldInputVector = GetWorldInputVectorProjectedToSurface();
		MovementForce += MovementSpeed * WorldInputVector;
		
		FVector WorldInputVectorNormalized = WorldInputVector;
		WorldInputVectorNormalized.Normalize();
		MovementForce += SustainedBoostForceHorizontal * WorldInputVectorNormalized;
	}

	UAsyncTickFunctions::ATP_AddForce(CollisionCapsule.Get(), MovementForce / DeltaTime, true, NAME_None);
	//CollisionCapsule->AddForce(DeltaTime * MovementForce, NAME_None, true);
}

void AMech::UpdateDrag(float DeltaTime)
{
	FVector Velocity = UAsyncTickFunctions::ATP_GetLinearVelocity(CollisionCapsule.Get());

	bool IsGrounded = (GroundHitResult.bBlockingHit || GroundHitResult.bStartPenetrating);

	FVector UpVector = (IsGrounded ? GroundHitResult.Normal : FVector::UpVector);
	FVector VerticalVelocity = Velocity.ProjectOnToNormal(UpVector);
	FVector HorizontalVelocity = Velocity - VerticalVelocity;

	float HorizontalDrag = (IsGrounded ? PhysicsDataAsset->HorizontalDragGround : PhysicsDataAsset->HorizontalDragAir);
	float VerticalDrag = (IsGrounded ? PhysicsDataAsset->VerticalDragGround : PhysicsDataAsset->VerticalDragAir);

	FVector HorizontalDragForceVector = (HorizontalVelocity.Length() * HorizontalDrag) * -HorizontalVelocity.GetSafeNormal();
	FVector VerticalDragForceVector = (VerticalVelocity.Length() * VerticalDrag) * -VerticalVelocity.GetSafeNormal();
	FVector DragForce = HorizontalDragForceVector + VerticalDragForceVector;

	UAsyncTickFunctions::ATP_AddForce(CollisionCapsule.Get(), DragForce / DeltaTime, true, NAME_None);
	//CollisionCapsule->AddForce(DragForce, NAME_None, true);
}

FVector AMech::GetPivotWorldLocation(bool IsAsync)
{
	if (CollisionCapsule.IsValid() == false)
		return GetActorLocation();

	if (IsAsync)
	{
		FTransform Transform = UAsyncTickFunctions::ATP_GetTransform(CollisionCapsule.Get());

		FVector UpDirection = Transform.GetUnitAxis(EAxis::Z);
		FVector Result = Transform.GetLocation() - CollisionCapsule->GetUnscaledCapsuleHalfHeight() * UpDirection;
		return Result;
	}
	else
	{
		FVector UpDirection = GetTransform().GetUnitAxis(EAxis::Z);
		FVector Result = GetActorLocation() - CollisionCapsule->GetUnscaledCapsuleHalfHeight() * UpDirection;
		return Result;
	}
}

FVector AMech::GetControlDirection(FVector Direction, bool FlattenToHorizontalPlane)
{
	FVector Result = GetControlRotation().RotateVector(Direction);

	if (FlattenToHorizontalPlane)
	{
		Result.Z = 0;
		Result.Normalize();
	}

	return Result;
}

FVector AMech::GetWorldInputVectorProjectedToSurface()
{
	FVector WorldSpaceInput = MoveInput.X * FVector::RightVector + MoveInput.Y * FVector::ForwardVector;
	FVector HorizontalControlDirection = WorldSpaceInput.Length() * GetControlDirection(WorldSpaceInput, true);
	FVector Result = HorizontalControlDirection;

	if (GroundHitResult.bBlockingHit || GroundHitResult.bStartPenetrating)
		Result = FVector::VectorPlaneProject(Result, GroundHitResult.Normal);

	return Result;
}

void AMech::RegisterPreventMovementSource(int32 sourceID)
{
	if (PreventMovementSources.Contains(sourceID) == false)
		PreventMovementSources.Add(sourceID);
}

void AMech::UnregisterPreventMovementSource(int32 sourceID)
{
	if (PreventMovementSources.Contains(sourceID))
		PreventMovementSources.Remove(sourceID);
}