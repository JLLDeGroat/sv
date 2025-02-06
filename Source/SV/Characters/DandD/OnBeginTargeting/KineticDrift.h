// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/OnBeginTargetingBase.h"
#include "KineticDrift.generated.h"

/**
 *
 */
UCLASS()
class SV_API UKineticDrift : public UOnBeginTargetingBase
{
    GENERATED_BODY()

public:
    UKineticDrift(const FObjectInitializer &ObjectInitializer);

    virtual FOnBeginTargetingOutput ActivateDD(FOnBeginTargetingInput &input) override;
};
