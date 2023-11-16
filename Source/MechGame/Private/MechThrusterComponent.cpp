// Fill out your copyright notice in the Description page of Project Settings.


#include "MechThrusterComponent.h"
#include "Mech.h"
#include "ThrusterEquipmentAsset.h"

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
		float InputAmount = FMath::Clamp(Mech->MoveInput.Length(), 0, 1);
		FBoostParameters SustainedBoostParams = ThrusterAsset->GetSustainedBoostParams(InputAmount);
		Mech->SustainedBoostForceHorizontal = SustainedBoostParams.HorizontalForce;
		Mech->SustainedBoostForceVertical = SustainedBoostParams.VerticalForce;
	}
	else
	{
		Mech->SustainedBoostForceHorizontal = 0.f;
		Mech->SustainedBoostForceVertical = 0.f;
	}
}

void UMechThrusterComponent::OnBoostInputStarted()
{
	if (bDashActive)
		return;

	DashPrepareTimer = 0;
	DashBoostTimer = 0;
	bDashActive = true;
	bDashInitialized = false;

	Mech->SustainedBoostForceHorizontal = 0.f;
	Mech->SustainedBoostForceVertical = 0.f;

	if (GEngine)
	{
		int32 MyID = GetUniqueID();
		GEngine->AddOnScreenDebugMessage(MyID, ThrusterAsset->DashPrepareTime, FColor::Red, FString::Printf(TEXT("Dash Started")));
	}
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
		Mech->RegisterPreventMovementSource(GetUniqueID());
		DashPrepareTimer += DeltaTime;
		return;
	}

	Mech->UnregisterPreventMovementSource(GetUniqueID());

	auto RootComponent = Cast<UPrimitiveComponent>(Mech->GetRootComponent());

	if (bDashInitialized == false)
	{
		bDashInitialized = true;

		// perform vertical velocity cancel
		FVector Velocity = Mech->GetVelocity();
		Velocity.Z *= (1.0f - ThrusterAsset->DashVerticalVelocityCancelAmount);
		RootComponent->SetPhysicsLinearVelocity(Velocity, false, NAME_None);

		// generate dash boost params
		float InputAmount = FMath::Clamp(Mech->MoveInput.Length(), 0, 1);
		DashBoostParams = ThrusterAsset->GetDashBoostParams(InputAmount);

		// cache input world direction
		if (InputAmount > FLT_EPSILON)
			DashBoostDirection = Mech->GetWorldInputVectorProjectedToSurface();
		else
			DashBoostDirection = Mech->GetActorForwardVector();
	}

	float DashT = DashBoostTimer / ThrusterAsset->DashLength;
	float CurveVal = ThrusterAsset->DashForceCurve.GetRichCurve()->Eval(DashT);

	FVector DashBoostForce = CurveVal * DashBoostParams.HorizontalForce * DashBoostDirection;
	DashBoostForce.Z += CurveVal * DashBoostParams.VerticalForce;
	Mech->DashBoostForce = DashBoostForce;

	if (GEngine)
	{
		int32 MyID = GetUniqueID();
		GEngine->AddOnScreenDebugMessage(MyID, DeltaTime, FColor::Red, FString::Printf(TEXT("Dash Boost %s"), *DashBoostForce.ToString()));
	}

	DashBoostTimer += DeltaTime;

	if (DashBoostTimer >= ThrusterAsset->DashLength)
	{
		bDashActive = false;
		Mech->DashBoostForce = FVector::ZeroVector;

		if (GEngine)
		{
			int32 MyID = GetUniqueID();
			GEngine->AddOnScreenDebugMessage(MyID, 2.f, FColor::Red, FString::Printf(TEXT("Dash Boost End")));
		}
	}
}
