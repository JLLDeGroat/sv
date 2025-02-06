// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FOnBeginTargetingInput.generated.h"

/**
 *
 */
USTRUCT()
struct FOnBeginTargetingInput
{
    GENERATED_BODY()
public:
    FOnBeginTargetingInput()
    {
    }

    static FOnBeginTargetingInput Create(AActor* actor)
    {
        auto input = FOnBeginTargetingInput();
        input.Actor = actor;
        return input;
    }

protected:
    UPROPERTY()
    AActor *Actor;
};
