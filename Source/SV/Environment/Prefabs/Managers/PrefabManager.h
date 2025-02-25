// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PrefabManager.generated.h"

/**
 *
 */
UCLASS()
class SV_API UPrefabManager : public UObject
{
    GENERATED_BODY()

public:
    UPrefabManager();

    bool GetBuildingPrefab(int xSize, int ySize, FString &foundBlueprint);
    bool GetRandomBuildingPrefab(int &xSize, int &ySize, FString &foundBlueprint);

    bool GetRandomExteriorPrefab(int &xSize, int &ySize, FString &foundBlueprint);

private:
    UPROPERTY()
    FRandomStream RandomStream;
};
