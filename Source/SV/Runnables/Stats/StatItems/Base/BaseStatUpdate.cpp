// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseStatUpdate.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../../Utilities/SvUtilities.h"
#include "../../../../Data/Game/FCurrentGameData.h"
#include "../../../../Characters/Components/CharacterDetailsComponent.h"
#include "../../../../Data/Game/Mission/FMissionStats.h"

void UBaseStatUpdate::ActivateStatUpdate() {
	UDebugMessages::LogError(this, "failed to run stat, or ran super.. dont do that");
}

FCrewMemberStats* UBaseStatUpdate::GetCrewMemberStats() {
	auto currentData = USvUtilities::GetCurrentGameData(GetWorld());
	if (currentData && StatActor && StatActor->GetComponentByClass<UCharacterDetailsComponent>()) {
		auto characterDetailsComponent = StatActor->GetComponentByClass<UCharacterDetailsComponent>();
		auto crewId = characterDetailsComponent->GetCharacterId();
		auto crewMember = currentData->GetCrewMember(crewId);
		if (crewMember)
			return crewMember->GetStats();
		else
			UDebugMessages::LogError(this, "failed to get stats for crew member");
	}
	return nullptr;
}

FMissionStats* UBaseStatUpdate::GetCurrentMissionStats() {
	auto currentData = USvUtilities::GetCurrentGameData(GetWorld());
	return currentData->GetCurrentMission()->GetMissionDetails()->GetMissionStats();
}