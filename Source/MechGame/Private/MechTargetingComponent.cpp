// Fill out your copyright notice in the Description page of Project Settings.

#include "MechTargetingComponent.h"
#include "ContextSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "ContextTargetComponent.h"
#include "Math/NumericLimits.h"
#include "Camera/CameraComponent.h"
#include "Mech.h"
#include "GameFramework/PlayerController.h"
#include "UnrealClient.h"

UMechTargetingComponent::UMechTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMechTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	Mech = Cast<AMech>(GetOwner());
}

void UMechTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ValidContextTargetsArray.Reset();
	ValidTargetingOptions.Reset();

	CalculateValidContextTargets();

	// filter results further in Blueprint / other C++ code if needed
	OnValidTargetingOptionsCollected.Broadcast();

	FilterTargetingOptions_LineOfSight();

	ScoreTargetingOptions();

	// all processing completed -> update UI / draw debug lines / other logic in BP or other C++ classes
	OnTargetingOptionsProcessingCompleted.Broadcast();

	UpdateWorldTargetLocation();
}

void UMechTargetingComponent::CalculateValidContextTargets()
{
	auto OwnerActor = Mech.Get();
	auto GameInstance = UGameplayStatics::GetGameInstance(OwnerActor);

	if (GameInstance == nullptr)
		return;

	auto ContextSubsystem = GameInstance->GetSubsystem<UContextSubsystem>();

	if (ContextSubsystem == nullptr)
		return;

	ContextSubsystem->GetContextTargetsOnLayers(&ValidContextTargetsArray, TargetContextLayerFlags);

	uint32 MyOwnerActorID = OwnerActor->GetUniqueID();

	for (int32 i = ValidContextTargetsArray.Num(); i-- > 0;)
	{
		auto Current = ValidContextTargetsArray[i];
		auto CurrentComponent = Current.Get();

		uint32 ComponentOwnerActorID = CurrentComponent->GetOwner()->GetUniqueID();

		if (ComponentOwnerActorID == MyOwnerActorID)
		{
			ValidContextTargetsArray.RemoveAt(i);
			continue;
		}

		auto NewTargetingOption = FTargetingOption();
		NewTargetingOption.IsValid = true;
		NewTargetingOption.ContextTargetComponent = CurrentComponent;

		ValidTargetingOptions.Add(NewTargetingOption);
	}
}

void UMechTargetingComponent::FilterTargetingOptions_LineOfSight()
{
	auto OwnerActor = Mech.Get();

	FVector TraceStartLocation = OwnerActor->GetActorLocation();

	int32 TargetsCount = ValidTargetingOptions.Num();

	for (int32 i = TargetsCount; i --> 0;)
	{
		FTargetingOption Target = ValidTargetingOptions[i];
		AActor* TargetActor = Target.ContextTargetComponent->GetOwner();

		TraceIgnoredActors.Add(OwnerActor);
		TraceIgnoredActors.Add(TargetActor);

		FVector TraceEnd = TargetActor->GetActorLocation();
		ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_PhysicsBody);
		bool TraceAgainstComplexGeo = false;
		bool IgnoreSelf = true;
		EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::ForOneFrame;

		bool bTraceHitFound = UKismetSystemLibrary::LineTraceSingle(
			this,
			TraceStartLocation,
			TraceEnd,
			TraceChannel,
			TraceAgainstComplexGeo,
			TraceIgnoredActors,
			DrawDebugType,
			TraceHitResult,
			IgnoreSelf);

		TraceIgnoredActors.Reset();

		if (bTraceHitFound)
		{
			ValidTargetingOptions.RemoveAt(i);
		}
	}
}

void UMechTargetingComponent::ScoreTargetingOptions()
{
	float MinDistance = TNumericLimits<float>::Max();
	float MaxDistance = 0.0f;

	FVector EvaluationSourceLocation = GetScoreEvaluationSourceLocation();
	FVector EvaluationForwardDirection = GetScoreEvaluationForwardDirection();

	int32 TargetsCount = ValidTargetingOptions.Num();

	FRichCurve* DotScoreCurve_Rich = DotScoreCurve.GetRichCurve();
	FRichCurve* DistanceScoreCurve_Rich = DistanceScoreCurve.GetRichCurve();

	for (int32 i = 0; i < TargetsCount; i++)
	{
		FTargetingOption* Target = &ValidTargetingOptions[i];
		FVector TargetActorLocation = Target->ContextTargetComponent->GetOwner()->GetActorLocation();
		FVector ToTarget = TargetActorLocation - EvaluationSourceLocation;

		float DotT = FVector::DotProduct(EvaluationForwardDirection, ToTarget.GetUnsafeNormal());
		Target->DotScore = DotScoreCurve_Rich->Eval(DotT);

		Target->DistanceToTargetingComponent = ToTarget.Length();

		if (Target->DistanceToTargetingComponent < MinDistance)
			MinDistance = Target->DistanceToTargetingComponent;

		if (Target->DistanceToTargetingComponent > MaxDistance)
			MaxDistance = Target->DistanceToTargetingComponent;
	}

	FVector2D DistanceMapInputRange = FVector2D(MinDistance, MaxDistance);
	FVector2D DistanceMapOutputRange = FVector2D(0.0f, 1.0f);

	for (int32 i = 0; i < TargetsCount; i++)
	{
		FTargetingOption* Target = &ValidTargetingOptions[i];

		float DistanceT = FMath::GetMappedRangeValueUnclamped(DistanceMapInputRange, DistanceMapOutputRange, Target->DistanceToTargetingComponent);
		Target->DistanceScore = DistanceScoreCurve_Rich->Eval(DistanceT);

		Target->TotalScore = Target->DotScore + Target->DistanceScore;
	}
}

FVector UMechTargetingComponent::GetScoreEvaluationSourceLocation()
{
	if (bUseCameraTransformForScoring && CameraComponent.IsValid())
	{
		return CameraComponent->GetComponentLocation();
	}

	return GetOwner()->GetActorLocation();
}

FVector UMechTargetingComponent::GetScoreEvaluationForwardDirection()
{
	if (bUseCameraTransformForScoring && CameraComponent.IsValid())
	{
		return CameraComponent->GetComponentTransform().GetUnitAxis(EAxis::X);
	}

	return GetOwner()->GetTransform().GetUnitAxis(EAxis::X);
}

FTargetingOption UMechTargetingComponent::GetBestTargetingOption()
{
	FTargetingOption Result = FTargetingOption();

	int32 TargetsCount = ValidTargetingOptions.Num();

	if (TargetsCount == 0)
		return Result;

	for (int32 i = 0; i < TargetsCount; i++)
	{
		FTargetingOption Current = ValidTargetingOptions[i];

		if (Result.IsValid == false)
		{
			Result = Current;
			continue;
		}

		if (Current.TotalScore > Result.TotalScore)
			Result = Current;
	}

	return Result;
}

void UMechTargetingComponent::RegisterPlayerReticleViewportLocation(const FVector2D& Location)
{
	ReticleScreenLocation = Location;
}

void UMechTargetingComponent::UpdateWorldTargetLocation()
{
	if (ValidTargetingOptions.Num() > 0)
	{
		FTargetingOption BestTarget = GetBestTargetingOption();
		WorldTargetLocation = BestTarget.GetLocation();
		return;
	}

	if (Mech->IsPlayerControlled())
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

		if (PlayerController == nullptr)
			return;

		FVector ReticleWorldLocation;
		FVector ReticleWorldDirection;

		PlayerController->DeprojectScreenPositionToWorld(ReticleScreenLocation.X, ReticleScreenLocation.Y, ReticleWorldLocation, ReticleWorldDirection);

		FVector TraceEnd = (ReticleWorldLocation + 100000 * ReticleWorldDirection);
		ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_PhysicsBody);
		bool TraceAgainstComplexGeo = false;
		bool IgnoreSelf = true;
		EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None;

		bool bTraceHitFound = UKismetSystemLibrary::LineTraceSingle(
			this,
			ReticleWorldLocation,
			TraceEnd,
			TraceChannel,
			TraceAgainstComplexGeo,
			TraceIgnoredActors,
			DrawDebugType,
			TraceHitResult,
			IgnoreSelf);

		WorldTargetLocation = (bTraceHitFound ? TraceHitResult.ImpactPoint : TraceEnd);
	}
}