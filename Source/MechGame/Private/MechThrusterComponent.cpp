// Fill out your copyright notice in the Description page of Project Settings.


#include "MechThrusterComponent.h"

UMechThrusterComponent::UMechThrusterComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMechThrusterComponent::BeginPlay()
{
	Super::BeginPlay();

	bDashActive = false;
	bWasBoostInputActivePreviousFrame = false;

	Mech = Cast<AMech>(GetOwner());
}

void UMechThrusterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Mech.IsValid() == false || ThrusterAsset == false)
		return;

	if (Mech->bBoostInputActive)
	{
		if (bWasBoostInputActivePreviousFrame == false)
			OnBoostInputStarted();
	}
	else
	{
		if (bWasBoostInputActivePreviousFrame)
			OnBoostInputCanceled();
	}

	bWasBoostInputActivePreviousFrame = Mech->bBoostInputActive;

	if (bDashActive)
	{
		UpdateDash(DeltaTime);
		return;
	}

	if (Mech->bBoostInputActive)
	{

	}
	else
	{

	}
}

void UMechThrusterComponent::OnBoostInputStarted()
{
	if (bDashActive)
		return;

	DashPrepareTimer = 0;
	DashBoostTimer = 0;
	bDashActive = true;
	bDashVelocityCancelPerformed = false;
}

void UMechThrusterComponent::OnBoostInputCanceled()
{
	if (bDashActive)
		return;
}

void UMechThrusterComponent::UpdateDash(float DeltaTime)
{
	if (DashPrepareTimer < ThrusterAsset->DashPrepareTime)
	{
		DashPrepareTimer += DeltaTime;
		return;
	}

	auto RootComponent = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());

	if (bDashVelocityCancelPerformed == false)
	{
		bDashVelocityCancelPerformed = true;

		FVector Velocity = RootComponent->GetComponentVelocity();
		Velocity.Z *= (1.0f - ThrusterAsset->DashVerticalVelocityCancelAmount);
		RootComponent->SetPhysicsLinearVelocity(Velocity, false, NAME_None);
	}

	float InputAmount = FMath::Clamp(Mech->MoveInput.Length(), 0, 1);
	FBoostParameters DashBoostParams = ThrusterAsset->GetDashBoostParams(InputAmount);

	float DashT = DashBoostTimer / ThrusterAsset->DashLength;

	FVector ControlDirectionForward = Mech->GetControlDirection(FVector::ForwardVector, true);
	FVector ControlDirectionRight = Mech->GetControlDirection(FVector::RightVector, true);

	DashBoostTimer += DeltaTime;

	if (DashBoostTimer >= ThrusterAsset->DashLength)
		bDashActive = false;
}
