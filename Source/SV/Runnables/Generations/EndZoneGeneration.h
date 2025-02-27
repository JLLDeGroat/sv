// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseGenerations.h"
#include "EndZoneGeneration.generated.h"

/**
 *
 */
UCLASS()
class SV_API UEndZoneGeneration : public UBaseGenerations
{
    GENERATED_BODY()

public:
    virtual UBaseGenerations *Generate() override;

    UEndZoneGeneration *SetSpawnLocations(TArray<FVector> locations);

protected:
    TArray<FVector> SpawnLocations;
};
