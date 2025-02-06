// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/OnGunFireBase.h"
#include "TrickleDownBallistics.generated.h"

/**
 *
 */
UCLASS()
class SV_API UTrickleDownBallistics : public UOnGunFireBase
{
    GENERATED_BODY()

public:
    UTrickleDownBallistics(const FObjectInitializer &ObjectInitializer);

    virtual FOnGunFireOutput ActivateDD(FOnGunFireInput &input) override;
};
