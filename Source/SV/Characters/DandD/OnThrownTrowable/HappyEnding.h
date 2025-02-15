// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/OnThrowableThrownBase.h"
#include "HappyEnding.generated.h"

/**
 *
 */
UCLASS()
class SV_API UHappyEnding : public UOnThrowableThrownBase
{
    GENERATED_BODY()

public:
    UHappyEnding(const FObjectInitializer &ObjectInitializer);

    virtual FOnThrowableThrownOutput ActivateDD(FOnThrowableThrownInput &input) override;
};
