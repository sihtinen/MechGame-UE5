// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MechDataStructures.h"
#include "MechTargetingComponent.generated.h"

class AMech;
class UContextTargetComponent;
class UCameraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnValidTargetingOptionsCollected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTargetingOptionsProcessingCompleted);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MECHGAME_API UMechTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = EContextLayers))
	int32 TargetContextLayerFlags = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRuntimeFloatCurve DotScoreCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRuntimeFloatCurve DistanceScoreCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseCameraTransformForScoring;

	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(BlueprintReadWrite)
	TArray<FTargetingOption> ValidTargetingOptions;

	UPROPERTY(BlueprintAssignable)
	FOnValidTargetingOptionsCollected OnValidTargetingOptionsCollected;

	UPROPERTY(BlueprintAssignable)
	FOnTargetingOptionsProcessingCompleted OnTargetingOptionsProcessingCompleted;

	UPROPERTY(BlueprintReadOnly)
	FVector WorldTargetLocation;

public:

	UMechTargetingComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	FTargetingOption GetBestTargetingOption();

	UFUNCTION(BlueprintCallable)
	void RegisterPlayerReticleViewportLocation(const FVector2D& ReticleViewportLocation);

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY()
	TWeakObjectPtr<AMech> Mech;

	UPROPERTY()
	TArray<TWeakObjectPtr<UContextTargetComponent>> ValidContextTargetsArray;

	UPROPERTY()
	TArray<AActor*> TraceIgnoredActors;

	FHitResult TraceHitResult;

	FVector2D ReticleScreenLocation;

private:

	void CalculateValidContextTargets();

	void FilterTargetingOptions_LineOfSight();

	void ScoreTargetingOptions();

	FVector GetScoreEvaluationSourceLocation();

	FVector GetScoreEvaluationForwardDirection();

	void UpdateWorldTargetLocation();
};
