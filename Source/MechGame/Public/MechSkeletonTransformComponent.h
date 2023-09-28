// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MechSkeletonTransformComponent.generated.h"

class AMech;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MECHGAME_API UMechSkeletonTransformComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skeleton Transform Component Settings")
	float PositionInterpSpeedGround;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skeleton Transform Component Settings")
	float PositionInterpSpeedAir;

public:
	UMechSkeletonTransformComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TWeakObjectPtr<AMech> Mech;

	UPROPERTY()
	TWeakObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
};
