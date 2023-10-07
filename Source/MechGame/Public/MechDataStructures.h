// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

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