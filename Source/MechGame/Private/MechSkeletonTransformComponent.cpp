// Fill out your copyright notice in the Description page of Project Settings.

#include "MechSkeletonTransformComponent.h"
#include "Mech.h"

UMechSkeletonTransformComponent::UMechSkeletonTransformComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PositionInterpSpeedGround = 5.f;
	PositionInterpSpeedAir = 3.f;
}

void UMechSkeletonTransformComponent::BeginPlay()
{
	Super::BeginPlay();

	Mech = Cast<AMech>(GetOwner());
	SkeletalMeshComponent = Cast<USkeletalMeshComponent>(Mech->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
}


// Called every frame
void UMechSkeletonTransformComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Mech.IsValid() == false || SkeletalMeshComponent.IsValid() == false)
		return;

	FVector MechLocation = Mech->GetActorLocation();
	FVector TargetLocation;
	float InterpolationSpeed;

	if (Mech->GroundHitResult.bBlockingHit)
	{
		float DistanceToGround = (Mech->GroundHitResult.ImpactPoint - MechLocation).Length();
		TargetLocation = FVector(0, 0, -DistanceToGround);
		InterpolationSpeed = PositionInterpSpeedGround;
	}
	else if (Mech->GroundHitResult.bStartPenetrating)
	{
		float DistanceToGround = (Mech->GetPivotWorldLocation() + Mech->GroundHitResult.PenetrationDepth * FVector::UpVector).Length();
		TargetLocation = FVector(0, 0, DistanceToGround);
		InterpolationSpeed = PositionInterpSpeedGround;
	}
	else
	{
		float DistanceToPivotPos = (Mech->GetPivotWorldLocation() - MechLocation).Length();
		TargetLocation = FVector(0, 0, -DistanceToPivotPos);
		InterpolationSpeed = PositionInterpSpeedAir;
	}

	FVector CurrentSkeletonPosition = SkeletalMeshComponent->GetRelativeLocation();
	CurrentSkeletonPosition = FMath::VInterpTo(CurrentSkeletonPosition, TargetLocation, DeltaTime, InterpolationSpeed);
	SkeletalMeshComponent->SetRelativeLocation(CurrentSkeletonPosition);
}

