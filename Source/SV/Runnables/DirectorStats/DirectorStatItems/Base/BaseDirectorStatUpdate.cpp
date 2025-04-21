// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseDirectorStatUpdate.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../../Utilities/SvUtilities.h"
#include "../../../../Data/Game/FCurrentGameData.h"
#include "../../../../Characters/Components/CharacterDetailsComponent.h"
#include "../../../../Data/Game/Mission/FMissionStats.h"

void UBaseDirectorStatUpdate::ActivateStatUpdate()
{
    UDebugMessages::LogError(this, "failed to run stat, or ran super.. dont do that");
}

void UBaseDirectorStatUpdate::CreateStatItem()
{
    auto directorManager = USvUtilities::GetGameModeDirectorManager(GetWorld());
    auto result = directorManager->TryAddCrewStat(StatActor);
    if (!result)
        return UDebugMessages::LogError(this, "failed to create stat item");
}
FDirectorCrewStats *UBaseDirectorStatUpdate::GetCrewStats()
{
    auto directorManager = USvUtilities::GetGameModeDirectorManager(GetWorld());

    auto currentStats = directorManager->GetCrewStats(StatActor);
    if (!currentStats)
    {
        CreateStatItem();
        currentStats = directorManager->GetCrewStats(StatActor);
    }

    return currentStats;
}