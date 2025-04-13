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
        Details.Emplace(FPrefabDetails("/Script/Engine.Blueprint'/Game/Environment/Prefabs/Fences/Fence2_Bp.Fence2_Bp_C'", 3, 4));
        Details.Emplace(FPrefabDetails("/Script/Engine.Blueprint'/Game/Environment/Prefabs/Fences/Fence3_Bp.Fence3_Bp_C'", 2, 4));
        Details.Emplace(FPrefabDetails("/Script/Engine.Blueprint'/Game/Environment/Prefabs/Fences/Fence4_Bp.Fence4_Bp_C'", 2, 4));
    }
    TArray<FPrefabDetails> GetDetails() { return Details; }

protected:
    UPROPERTY()
    TArray<FPrefabDetails> Details;
};
