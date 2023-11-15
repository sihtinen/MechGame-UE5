// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MechDataStructures.generated.h"

class UContextTargetComponent;
class UProjectileAsset;

UENUM(BlueprintType)
enum class EEquipmentSlotType : uint8 {

    LeftShoulder    UMETA(DisplayName = "Left Shoulder"),
    RightShoulder   UMETA(DisplayName = "Right Shoulder"),
    LeftArm         UMETA(DisplayName = "Left Arm"),
    RightArm        UMETA(DisplayName = "Right Arm"),

    Melee           UMETA(DisplayName = "Melee"),
    Generator       UMETA(DisplayName = "Generator"),

    Head            UMETA(DisplayName = "Head"),
    Body            UMETA(DisplayName = "Body"),
    Arms            UMETA(DisplayName = "Arms"),
    Legs            UMETA(DisplayName = "Legs"),

    Utility1        UMETA(DisplayName = "Utility 1"),
    Utility2        UMETA(DisplayName = "Utility 2"),
    Utility3        UMETA(DisplayName = "Utility 3"),
    Utility4        UMETA(DisplayName = "Utility 4"),

    Passive1        UMETA(DisplayName = "Passive 1"),
    Passive2        UMETA(DisplayName = "Passive 2"),
    Passive3        UMETA(DisplayName = "Passive 3"),
    Passive4        UMETA(DisplayName = "Passive 4"),

    MAX             UMETA(Hidden),
};

USTRUCT(BlueprintType)
struct FTargetingOption
{
    GENERATED_BODY()

public:

    UPROPERTY(BlueprintReadWrite)
    bool IsValid;

    UPROPERTY(BlueprintReadWrite)
    TWeakObjectPtr<UContextTargetComponent> ContextTargetComponent;

    UPROPERTY(BlueprintReadWrite)
    float DotScore;

    UPROPERTY(BlueprintReadWrite)
    float DistanceScore;

    UPROPERTY(BlueprintReadWrite)
    float TotalScore;

    UPROPERTY(BlueprintReadWrite)
    float DistanceToTargetingComponent;

public:

    FTargetingOption();

    FVector GetLocation();
    FVector GetVelocity();
};

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EContextLayers : uint8
{
    NONE = 0 UMETA(Hidden),

    Faction_0 = 1 << 0,
    Faction_1 = 1 << 1,
    Faction_2 = 1 << 2,
    Faction_3 = 1 << 3,

    Faction_Player = 1 << 4,

    EnvironmentObstacle = 1 << 5,
    LineOfSightObstacle = 1 << 6,

    WeaponHazard = 1 << 7,
};
ENUM_CLASS_FLAGS(EContextLayers);

USTRUCT(BlueprintType)
struct FProjectileState
{
    GENERATED_BODY()

public:

    UPROPERTY(BlueprintReadOnly)
    float AliveTime;

    UPROPERTY(BlueprintReadOnly)
    FVector Location;

    UPROPERTY(BlueprintReadOnly)
    FVector ForwardDirection;

    UPROPERTY(BlueprintReadOnly)
    FVector Velocity;

    UPROPERTY(BlueprintReadOnly)
    TWeakObjectPtr<AActor> OwnerActor;

    UPROPERTY(BlueprintReadOnly)
    TWeakObjectPtr<class APooledActor> VisualActor;

    UPROPERTY(BlueprintReadOnly)
    TWeakObjectPtr<class UContextTargetComponent> TargetComponent;

    UPROPERTY(BlueprintReadOnly)
    TWeakObjectPtr<UProjectileAsset> ProjectileAsset;

public:

    FProjectileState();
    FProjectileState(AActor* OwnerActor, UProjectileAsset* Asset);

};

USTRUCT(BlueprintType)
struct FAccuracyConfig
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float LossPerUse = 0.2;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float LossOverVelocityRate = 0.2;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float MaxVelocity = 0.2;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float BaseRegainSpeed = 0.5;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FRuntimeFloatCurve RegainRateCurve = FRuntimeFloatCurve();

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FRuntimeFloatCurve LossOverMaxVelocityCurve = FRuntimeFloatCurve();

public:

    void UpdateTick(float& Accuracy, const float& DeltaTime, const float& VelocityMagnitude);

    void ApplyUsageLoss(float& Accuracy);
};

USTRUCT(BlueprintType)
struct FWeaponInaccuracyNoise
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FVector NoiseSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FVector NoiseScale;

public:

    void ApplyToDirection(FVector& Direction, const float Accuracy, const float GameTime);

};