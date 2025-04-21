// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../../../Enums/EWorldEnums.h"
#include "../../../../GameModes/Managers/DirectorManager.h"
#include "BaseDirectorStatUpdate.generated.h"

class UDirectorManager;
/**
 *
 */
UCLASS()
class SV_API UBaseDirectorStatUpdate : public UObject
{
    GENERATED_BODY()

public:
    UBaseDirectorStatUpdate *Setup(AActor *statActor, EDirectorStatType sType, float value = 0.0f)
    {
        StatActor = statActor;
        StatType = sType;
        StatValue = value;
        return this;
    }

    virtual void ActivateStatUpdate();

protected:
    UPROPERTY()
    AActor *StatActor;
    UPROPERTY()
    EDirectorStatType StatType;
    UPROPERTY()
    float StatValue;

    FDirectorCrewStats *GetCrewStats();
    void CreateStatItem();
};
