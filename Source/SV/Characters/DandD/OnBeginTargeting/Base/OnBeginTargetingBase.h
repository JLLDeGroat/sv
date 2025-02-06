// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../../Base/BaseDD.h"
#include "../Models/FOnBeginTargetingOutput.h"
#include "../Models/FOnBeginTargetingInput.h"
#include "OnBeginTargetingBase.generated.h"

/**
 *
 */
UCLASS()
class SV_API UOnBeginTargetingBase : public UBaseDD
{
    GENERATED_BODY()

public:
    UOnBeginTargetingBase(const FObjectInitializer &ObjectInitializer) : UBaseDD(ObjectInitializer) {}

    virtual FOnBeginTargetingOutput ActivateDD(FOnBeginTargetingInput &input)
    {
        return FOnBeginTargetingOutput();
    }
};
