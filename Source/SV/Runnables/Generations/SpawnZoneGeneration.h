// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseGenerations.h"
#include "SpawnZoneGeneration.generated.h"

/**
 *
 */
UCLASS()
class SV_API USpawnZoneGeneration : public UBaseGenerations
{
    GENERATED_BODY()

public:
    virtual UBaseGenerations *Generate() override;

    USpawnZoneGeneration* SetSpawnLocations(TArray<FVector> locations);

protected:
    TArray<FVector> SpawnLocations;
};
