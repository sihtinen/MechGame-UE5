// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MechPoseSyncComponent.generated.h"

class UPoseableMeshComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MECHGAME_API UMechPoseSyncComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMechPoseSyncComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetSkeletalMesh(USkeletalMesh* SkeletalMesh);

protected:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<USkeletalMesh> PoseSkeletalMesh;

	UPROPERTY()
	TWeakObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY()
	TWeakObjectPtr<UPoseableMeshComponent> PoseableMeshComponent;

	TMap<FName, FTransform> SourceSkeletonReferencePose;
	TMap<FName, FTransform> PoseableSkeletonOffsets;
};
