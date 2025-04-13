// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FPrefabDetails.h"
#include "FBuildingPrefabData.generated.h"

/**
 *
 */
USTRUCT()
struct FBuildingPrefabData
{
    GENERATED_BODY()
public:
    FBuildingPrefabData()
    {
        Details.Emplace(FPrefabDetails("/Script/Engine.Blueprint'/Game/Environment/Prefabs/Buildings/Building1_Bp.Building1_Bp_C'", 4, 6));
        Details.Emplace(FPrefabDetails("/Script/Engine.Blueprint'/Game/Environment/Prefabs/Buildings/Building2_Bp.Building2_Bp_C'", 4, 6));
        Details.Emplace(FPrefabDetails("/Script/Engine.Blueprint'/Game/Environment/Prefabs/Buildings/Building3_Bp.Building3_Bp_C'", 7, 6));
        Details.Emplace(FPrefabDetails("/Script/Engine.Blueprint'/Game/Environment/Prefabs/Buildings/Building4_Bp.Building4_Bp_C'", 7, 9));
        Details.Emplace(FPrefabDetails("/Script/Engine.Blueprint'/Game/Environment/Prefabs/Buildings/Building5_Bp.Building5_Bp_C'", 7, 9));
        Details.Emplace(FPrefabDetails("/Script/Engine.Blueprint'/Game/Environment/Prefabs/Buildings/Building6_Bp.Building6_Bp_C'", 4, 6));
    }
    TArray<FPrefabDetails> GetDetails() { return Details; }

protected:
    UPROPERTY()
    TArray<FPrefabDetails> Details;
};
