// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseDirectorGameType.h"
#include "TraversalDirector.generated.h"

/**
 *
 */
UCLASS()
class SV_API UTraversalDirector : public UBaseDirectorGameType
{
    GENERATED_BODY()

public:
    UTraversalDirector(const FObjectInitializer &ObjectInitializer);
    virtual float GeneratePreassure() override;
protected:
private:
};
