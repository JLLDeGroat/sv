// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/OnGunFireBase.h"
#include "LeadTax.generated.h"

/**
 *
 */
UCLASS()
class SV_API ULeadTax : public UOnGunFireBase
{
    GENERATED_BODY()

public:
    ULeadTax(const FObjectInitializer &ObjectInitializer);

    virtual FOnGunFireOutput ActivateDD(FOnGunFireInput &input) override;
};
