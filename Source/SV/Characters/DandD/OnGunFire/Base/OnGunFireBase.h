// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../../Base/BaseDD.h"
#include "../Models/FOnGunFireInput.h"
#include "../Models/FOnGunFireOutput.h"
#include "OnGunFireBase.generated.h"

/**
 *
 */
UCLASS()
class SV_API UOnGunFireBase : public UBaseDD
{
    GENERATED_BODY()

public:
    UOnGunFireBase(const FObjectInitializer &ObjectInitializer) : UBaseDD(ObjectInitializer) {}

    virtual FOnGunFireOutput ActivateDD(FOnGunFireInput &input)
    {
        return FOnGunFireOutput();
    }
};
