// Fill out your copyright notice in the Description page of Project Settings.

#include "PrefabManager.h"
#include "../Data/FBuildingPrefabData.h"
#include "../Data/FExteriorPrefabData.h"

UPrefabManager::UPrefabManager()
{
    RandomStream = FRandomStream(FDateTime::Now().GetMillisecond());
}

bool UPrefabManager::GetBuildingPrefab(int xSize, int ySize, FString &foundBlueprint)
{
    auto data = FBuildingPrefabData();
    auto details = data.GetDetails();

    foundBlueprint = "INVALID";

    TArray<FString> blueprints;

    for (FPrefabDetails detail : details)
        if (detail.GetX() <= xSize && detail.GetY() <= ySize)
            blueprints.Emplace(detail.GetBlueprintLocation());

    if (blueprints.Num() == 0)
        return false;

    foundBlueprint = blueprints[FMath::RandRange(0, blueprints.Num() - 1)];
    return true;
}

bool UPrefabManager::GetRandomBuildingPrefab(int &xSize, int &ySize, FString &foundBlueprint)
{
    auto data = FBuildingPrefabData();
    auto details = data.GetDetails();

    if (details.Num() == 0)
        return false;

    auto foundItem = details[RandomStream.RandRange(0, details.Num() - 1)];
    xSize = foundItem.GetX();
    ySize = foundItem.GetY();
    foundBlueprint = foundItem.GetBlueprintLocation();
    return true;
}

bool UPrefabManager::GetRandomExteriorPrefab(int &xSize, int &ySize, FString &foundBlueprint)
{
    auto data = FExteriorPrefabData();
    auto details = data.GetDetails();

    if (details.Num() == 0)
        return false;

    auto foundItem = details[RandomStream.RandRange(0, details.Num() - 1)];
    xSize = foundItem.GetX();
    ySize = foundItem.GetY();
    foundBlueprint = foundItem.GetBlueprintLocation();
    return true;
}