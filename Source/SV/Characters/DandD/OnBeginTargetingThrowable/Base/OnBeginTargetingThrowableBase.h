// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../../Base/BaseDD.h"
#include "../Models/FOnBeginTargetingThrowableInput.h"
#include "../Models/FOnBeginTargetingThrowableOutput.h"
#include "OnBeginTargetingThrowableBase.generated.h"

/**
 *
 */
UCLASS()
class SV_API UOnBeginTargetingThrowableBase : public UBaseDD
{
    GENERATED_BODY()

public:
    UOnBeginTargetingThrowableBase(const FObjectInitializer &ObjectInitializer) : UBaseDD(ObjectInitializer) {}

    virtual FOnBeginTargetingThrowableOutput ActivateDD(FOnBeginTargetingThrowableInput &input)
    {
        return FOnBeginTargetingThrowableOutput();
    }
};
