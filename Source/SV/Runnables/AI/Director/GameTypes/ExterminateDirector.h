// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseDirectorGameType.h"
#include "ExterminateDirector.generated.h"

/**
 *
 */
UCLASS()
class SV_API UExterminateDirector : public UBaseDirectorGameType
{
    GENERATED_BODY()

public:
    UExterminateDirector(const FObjectInitializer &ObjectInitializer);
    virtual float GeneratePreassure() override;

protected:
private:
};
