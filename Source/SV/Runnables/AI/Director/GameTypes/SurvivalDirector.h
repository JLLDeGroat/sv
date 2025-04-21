// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseDirectorGameType.h"
#include "SurvivalDirector.generated.h"

/**
 *
 */
UCLASS()
class SV_API USurvivalDirector : public UBaseDirectorGameType
{
    GENERATED_BODY()

public:
    USurvivalDirector(const FObjectInitializer &ObjectInitializer);
    virtual float GeneratePreassure() override;
    virtual void SpawnRequiredEnemies() override;

protected:
private:
};
