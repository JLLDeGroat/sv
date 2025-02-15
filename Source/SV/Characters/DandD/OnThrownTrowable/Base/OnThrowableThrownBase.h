// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../../Base/BaseDD.h"
#include "../Models/FOnThrowableThrownOutput.h"
#include "../Models/FOnThrowableThrownInput.h"
#include "OnThrowableThrownBase.generated.h"

/**
 *
 */
UCLASS()
class SV_API UOnThrowableThrownBase : public UBaseDD
{
    GENERATED_BODY()

public:
    UOnThrowableThrownBase(const FObjectInitializer &ObjectInitializer) : UBaseDD(ObjectInitializer) {}

    virtual FOnThrowableThrownOutput ActivateDD(FOnThrowableThrownInput &input)
    {
        return FOnThrowableThrownOutput();
    }
};
