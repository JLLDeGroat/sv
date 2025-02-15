// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/OnBeginTargetingThrowableBase.h"
#include "SizeMatters.generated.h"

/**
 *
 */
UCLASS()
class SV_API USizeMatters : public UOnBeginTargetingThrowableBase
{
    GENERATED_BODY()

public:
    USizeMatters(const FObjectInitializer &ObjectInitializer);

    virtual FOnBeginTargetingThrowableOutput ActivateDD(FOnBeginTargetingThrowableInput &input) override;
};
