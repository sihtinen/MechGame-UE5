// Fill out your copyright notice in the Description page of Project Settings.

#include "MechPoseSyncComponent.h"
#include "Components/PoseableMeshComponent.h"

UMechPoseSyncComponent::UMechPoseSyncComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UMechPoseSyncComponent::InitializeComponent()
{
	Super::InitializeComponent();

	AActor* OwnerActor = GetOwner();
	SkeletalMeshComponent = Cast<USkeletalMeshComponent>(OwnerActor->GetComponentByClass(USkeletalMeshComponent::StaticClass()));

	int32 NumBones = SkeletalMeshComponent->GetNumBones();
	for (int32 i = 0; i < NumBones; i++)
	{
		FName BoneName = SkeletalMeshComponent->GetBoneName(i);
		FTransform BoneTransform = SkeletalMeshComponent->GetBoneTransform(BoneName, RTS_Component);
		SourceSkeletonReferencePose.Add(BoneName, BoneTransform);
	}
}

void UMechPoseSyncComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();
	bool UseManualAttachment = true;

	PoseableMeshComponent = Cast<UPoseableMeshComponent>(OwnerActor->AddComponentByClass(UPoseableMeshComponent::StaticClass(), UseManualAttachment, FTransform(), true));
	PoseableMeshComponent->SetupAttachment(SkeletalMeshComponent.Get());

	OwnerActor->FinishAddComponent(PoseableMeshComponent.Get(), UseManualAttachment, FTransform());
	OwnerActor->AddInstanceComponent(PoseableMeshComponent.Get());
}

void UMechPoseSyncComponent::SetSkeletalMesh(USkeletalMesh* SkeletalMesh)
{
	if (!SkeletalMesh || SkeletalMeshComponent.IsValid() == false || PoseableMeshComponent.IsValid() == false)
		return;

	PoseSkeletalMesh = SkeletalMesh;

	bool bReinitPose = true;
	PoseableMeshComponent->SetSkeletalMesh(PoseSkeletalMesh, bReinitPose);

	int32 NumPoseableBones = PoseableMeshComponent->GetNumBones();
	for (int32 i = 0; i < NumPoseableBones; i++)
	{
		FName PoseableBoneName = PoseableMeshComponent->GetBoneName(i);

		FTransform PoseableBoneTransform = PoseableMeshComponent->GetBoneTransform(PoseableBoneName, RTS_Component);
		FTransform SourceBoneTransform = SourceSkeletonReferencePose[PoseableBoneName];

		FVector LocationOffset = PoseableBoneTransform.GetLocation() - SourceBoneTransform.GetLocation();
		FTransform OffsetTransform = FTransform(LocationOffset);

		PoseableSkeletonOffsets.Add(PoseableBoneName, OffsetTransform);
	}
}

void UMechPoseSyncComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PoseSkeletalMesh.IsNull() || SkeletalMeshComponent.IsValid() == false || PoseableMeshComponent.IsValid() == false)
		return;

	FTransform OwnerTransform = GetOwner()->GetTransform();

	int32 PoseableBoneCount = PoseableMeshComponent->GetNumBones();
	for (int32 i = 0; i < PoseableBoneCount; i++)
	{
		FName PoseableBoneName = PoseableMeshComponent->GetBoneName(i);

		FTransform SourceBoneTransform = SkeletalMeshComponent->GetBoneTransform(PoseableBoneName);
		FTransform OffsetTransform = PoseableSkeletonOffsets[PoseableBoneName];

		SourceBoneTransform.AddToTranslation(OwnerTransform.GetRotation() * OffsetTransform.GetLocation());

		PoseableMeshComponent->SetBoneTransformByName(PoseableBoneName, SourceBoneTransform, EBoneSpaces::WorldSpace);
	}
}