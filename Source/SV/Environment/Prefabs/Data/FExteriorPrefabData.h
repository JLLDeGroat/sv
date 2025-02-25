// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FPrefabDetails.h"
#include "FExteriorPrefabData.generated.h"

/**
 *
 */
USTRUCT()
struct FExteriorPrefabData
{
    GENERATED_BODY()
public:
    FExteriorPrefabData()
    {
        Details.Emplace(FPrefabDetails("/Script/Engine.Blueprint'/Game/Environment/Prefabs/Fences/Fence1_Bp.Fence1_Bp_C'", 3, 4));
    }
    TArray<FPrefabDetails> GetDetails() { return Details; }

protected:
    UPROPERTY()
    TArray<FPrefabDetails> Details;
};
