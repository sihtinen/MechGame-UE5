// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MechDataStructures.generated.h"

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
    float DotScore;
    float DistanceScore;
    float TotalScore;

public:
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