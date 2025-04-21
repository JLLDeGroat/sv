// Fill out your copyright notice in the Description page of Project Settings.

#include "DirectorManager.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Data/Game/FCurrentGameData.h"

UDirectorManager::UDirectorManager(const FObjectInitializer &ObjectInitializer)
    : UActorComponent(ObjectInitializer)
{
}

TArray<FDirectorCrewStats *> UDirectorManager::GetAllCrewStats()
{
    TArray<FDirectorCrewStats *> crewStats;
    for (FDirectorCrewStats &thisCrewStatItem : DirectorCrewStats)
        crewStats.Emplace(&thisCrewStatItem);

    return crewStats;
}

FDirectorCrewStats *UDirectorManager::GetCrewStats(AActor *actor)
{
    auto turn = GetTurn();
    FDirectorCrewStats *statItem = nullptr;
    for (FDirectorCrewStats &crewStats : DirectorCrewStats)
    {
        if (crewStats.GetTurn() == turn &&
            crewStats.GetStatOwner() == actor)
            statItem = &crewStats;
    }
    return statItem;
}

bool UDirectorManager::TryAddCrewStat(AActor *actor)
{
    auto turn = GetTurn();
    FDirectorCrewStats *statItem = nullptr;
    for (FDirectorCrewStats &crewStats : DirectorCrewStats)
    {
        if (crewStats.GetTurn() == turn &&
            crewStats.GetStatOwner() == actor)
            statItem = &crewStats;
    }

    if (statItem)
        return false; // already stat item

    auto crewStats = FDirectorCrewStats(actor, turn);
    DirectorCrewStats.Emplace(crewStats);
    return true;
}

int UDirectorManager::GetTurn()
{
    auto currentGameData = USvUtilities::GetCurrentGameData(GetWorld());
    if (!currentGameData)
    {
        UDebugMessages::LogError(this, "failed to get currentGameData");
        return 0;
    }
    auto currentMission = currentGameData->GetCurrentMission()->GetMissionDetails();
    auto turn = currentMission->GetTurn();
    return turn;
}

void UDirectorManager::MapAvailableSpawns(TArray<TPair<FVector, FVector>> availableMapItems)
{
    for (TPair<FVector, FVector> pair : availableMapItems)
    {
        WallSpawnLocations.Emplace(pair.Key, pair.Value);
    }
}
void UDirectorManager::SetAvailableFloorSpawns(TArray<FVector> availableFloorSpawns)
{
    FloorSpawnLocations = availableFloorSpawns;
}

TArray<FVector> UDirectorManager::GetFloorSpawnLocations()
{
    return FloorSpawnLocations;
}
TArray<FWallSpawnLoc> UDirectorManager::GetWallSpawnLocations()
{
    return WallSpawnLocations;
}