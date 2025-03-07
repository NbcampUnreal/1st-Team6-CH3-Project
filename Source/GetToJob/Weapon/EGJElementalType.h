// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EGJElementalType.generated.h"

UENUM(BlueprintType)
enum class EGJElementalType : uint8
{
    None    UMETA(DisplayName = "None"),
    Shock   UMETA(DisplayName = "Shock"),
    Freeze  UMETA(DisplayName = "Freeze"),
    Burn    UMETA(DisplayName = "Burn")
};

