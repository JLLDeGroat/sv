// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseDirectorGameType.h"
#include "ExtractionDirector.generated.h"

/**
 *
 */
UCLASS()
class SV_API UExtractionDirector : public UBaseDirectorGameType
{
    GENERATED_BODY()

public:
    UExtractionDirector(const FObjectInitializer &ObjectInitializer);
    virtual float GeneratePreassure() override;
protected:
private:
};
