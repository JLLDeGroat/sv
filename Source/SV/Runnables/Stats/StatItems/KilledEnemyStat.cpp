// Fill out your copyright notice in the Description page of Project Settings.


#include "KilledEnemyStat.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../Data/Game/Stats/FCrewMemberStats.h"
#include "../../../Data/Game/Mission/FMissionStats.h"
#include "../../../Data/Game/Mission/FMissionDetails.h"

void UKilledEnemyStat::ActivateStatUpdate() {

	if (!StatActor)
		return UDebugMessages::LogError(this, "This Stat is not functioning, stat actor is nullptr");

	auto crewMemberStats = GetCrewMemberStats();
	if (crewMemberStats) {
		crewMemberStats->AddToKills(1);
		UDebugMessages::LogError(this, "new kills " + FString::SanitizeFloat(crewMemberStats->GetKills()));
	}

	auto currentMission = GetCurrentMissionStats();
	if (currentMission)
		currentMission->AddToKilledAmount();
	else
		UDebugMessages::LogError(this, "failed to add to current mission stats");
}