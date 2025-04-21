// Fill out your copyright notice in the Description page of Project Settings.

#include "MovementStat.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../Data/Game/Stats/FCrewMemberStats.h"
#include "../../../Data/Game/Mission/FMissionStats.h"
#include "../../../Data/Game/Mission/FMissionDetails.h"

void UMovementStat::ActivateStatUpdate()
{
    if (!StatActor)
        return UDebugMessages::LogError(this, "This Stat is not functioning, stat actor is nullptr");

    auto crewMemberStats = GetCrewMemberStats();
    if (crewMemberStats)
    {
        crewMemberStats->AddToMovement(1);
        UDebugMessages::LogError(this, "new kills " + FString::SanitizeFloat(crewMemberStats->GetKills()));
    }

    // not adding to mission stats for movement, only a crew stat item
}