// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FOnBeginTargetingThrowableInput.generated.h"

/**
 *
 */
USTRUCT()
struct FOnBeginTargetingThrowableInput
{
    GENERATED_BODY()
public:
    FOnBeginTargetingThrowableInput()
    {
    }

    static FOnBeginTargetingThrowableInput Create(AActor *actor)
    {
        auto input = FOnBeginTargetingThrowableInput();
        input.Actor = actor;
        return input;
    }

protected:
    UPROPERTY()
    AActor *Actor;
};
