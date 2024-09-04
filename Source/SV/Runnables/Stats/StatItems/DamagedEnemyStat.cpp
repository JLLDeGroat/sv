// Fill out your copyright notice in the Description page of Project Settings.


#include "DamagedEnemyStat.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../Data/Game/Stats/FCrewMemberStats.h"


void UDamagedEnemyStat::ActivateStatUpdate() {
	if (!StatActor || StatValue <= 0)
		return UDebugMessages::LogError(this, "This Stat is not functioning, stat actor is nullptr");

	auto crewMemberStats = GetCrewMemberStats();
	if (crewMemberStats)
		crewMemberStats->AddToDamage(StatValue);
	else
		return UDebugMessages::LogError(this, "failed to get crew member stats item");
}