// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FOnThrowableThrownInput.generated.h"

/**
 *
 */
USTRUCT()
struct FOnThrowableThrownInput
{
    GENERATED_BODY()
public:
    FOnThrowableThrownInput()
    {
    }

    static FOnThrowableThrownInput Create(AActor *actor)
    {
        auto input = FOnThrowableThrownInput();
        input.Actor = actor;
        return input;
    }

protected:
    UPROPERTY()
    AActor *Actor;
};
