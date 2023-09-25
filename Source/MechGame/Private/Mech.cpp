// Fill out your copyright notice in the Description page of Project Settings.


#include "Mech.h"

AMech::AMech()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMech::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	FTransform Transform = GetTransform();
	bool UseManualAttachment = false;

	PrePhysicsTickComponent = Cast<UTickEventComponent>(AddComponentByClass(UTickEventComponent::StaticClass(), UseManualAttachment, Transform, true));
	FinishAddComponent(PrePhysicsTickComponent.Get(), UseManualAttachment, Transform);
	AddInstanceComponent(PrePhysicsTickComponent.Get());

	PostPhysicsTickComponent = Cast<UTickEventComponent>(AddComponentByClass(UTickEventComponent::StaticClass(), UseManualAttachment, Transform, true));
	FinishAddComponent(PostPhysicsTickComponent.Get(), UseManualAttachment, Transform);
	AddInstanceComponent(PostPhysicsTickComponent.Get());
}

void AMech::BeginPlay()
{
	Super::BeginPlay();
	
	PhysicsTraceIgnoredActors.Add(this);

	CollisionCapsule = Cast<UCapsuleComponent>(GetComponentByClass(UCapsuleComponent::StaticClass()));

	PrePhysicsTickComponent->OnTickEvent.AddDynamic(this, &AMech::OnPrePhysicsTick);
	PostPhysicsTickComponent->OnTickEvent.AddDynamic(this, &AMech::OnPostPhysicsTick);
}

void AMech::OnPrePhysicsTick(float DeltaTime)
{
	OnPrePhysicsTick_Implementation(DeltaTime);
	OnPrePhysicsTick_BP(DeltaTime);
	OnPrePhysicsTickEvent.Broadcast(DeltaTime);
}

void AMech::OnPrePhysicsTick_Implementation(float DeltaTime)
{
	if (PhysicsDataAsset == false)
		return;

	DoSurfaceCheck();
	UpdateRideHeightPID(DeltaTime);
	UpdateForwardDirection(DeltaTime);
}

void AMech::OnPostPhysicsTick(float DeltaTime)
{
	OnPostPhysicsTick_Implementation(DeltaTime);
	OnPostPhysicsTick_BP(DeltaTime);
	OnPostPhysicsTickEvent.Broadcast(DeltaTime);
}

void AMech::OnPostPhysicsTick_Implementation(float DeltaTime)
{

}

void AMech::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMech::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMech::DoSurfaceCheck()
{
	FVector PivotLocation = GetPivotWorldLocation();
	FVector UpDirection = GetTransform().GetUnitAxis(EAxis::Z);
	FVector TraceStart = PivotLocation + PhysicsDataAsset->SurfaceTraceHeight * UpDirection;
	FVector TraceEnd = TraceStart - PhysicsDataAsset->SurfaceTraceDistance * UpDirection;
	ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_PhysicsBody);
	bool TraceAgainstComplexGeo = false;
	bool IgnoreSelf = true;

	UKismetSystemLibrary::CapsuleTraceSingle(
		this,
		TraceStart,
		TraceEnd,
		PhysicsDataAsset->CollisionCapsuleRadius,
		PhysicsDataAsset->CollisionCapsuleHalfHeight,
		TraceChannel,
		TraceAgainstComplexGeo,
		PhysicsTraceIgnoredActors,
		EDrawDebugTrace::ForOneFrame,
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

	FVector PivotLocation = GetPivotWorldLocation();
	FVector UpDirection = GetTransform().GetUnitAxis(EAxis::Z);
	FVector HitLocation = GroundHitResult.bStartPenetrating ? (PivotLocation + GroundHitResult.PenetrationDepth * UpDirection) : GroundHitResult.ImpactPoint;

	float CurrentValue = FVector::Distance(HitLocation, PivotLocation);
	float TargetValue = PhysicsDataAsset->SurfaceRideHeight;

	RideHeightPIDState = RideHeightPID->UpdateTick(RideHeightPIDState, DeltaTime, CurrentValue, TargetValue);

	FVector Force = RideHeightPIDState.Output * UpDirection;

	CollisionCapsule->AddForce(Force, NAME_None, true);
}

void AMech::UpdateForwardDirection(float DeltaTime)
{
	float CurrentYaw = GetTransform().Rotator().GetNormalized().GetComponentForAxis(EAxis::Z);
	float ControlRotationYaw = GetControlRotation().GetNormalized().GetComponentForAxis(EAxis::Z);
	FVector RotateAxis = GetTransform().GetUnitAxis(EAxis::Z);

	ForwardDirectionPIDState = PhysicsUtils::ApplyAngleTorquePID(
		ForwardDirectionPID,
		CollisionCapsule.Get(),
		ForwardDirectionPIDState,
		RotateAxis,
		DeltaTime,
		CurrentYaw,
		ControlRotationYaw);
}

FVector AMech::GetPivotWorldLocation()
{
	if (CollisionCapsule.IsValid() == false)
		return GetActorLocation();

	FVector UpDirection = GetTransform().GetUnitAxis(EAxis::Z);
	FVector Result = GetActorLocation() - CollisionCapsule->GetUnscaledCapsuleHalfHeight() * UpDirection;
	return Result;
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