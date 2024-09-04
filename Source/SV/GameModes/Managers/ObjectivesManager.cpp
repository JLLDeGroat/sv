// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectivesManager.h"
#include "../../Delegates/HudDelegates.h"

#include "../../Instance/SvGameInstance.h"
#include "../../Instance/Managers/CurrentGameDataManager.h"
#include "../../Utilities/SvUtilities.h"

#include "VgCore/Domain/Debug/DebugMessages.h"

void UObjectivesManager::SetupMainObjective() {

	auto gameData = USvUtilities::GetCurrentGameData(GetWorld());
	if (!gameData)
		return UDebugMessages::LogError(this, "failed to get game data");

	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "failed to get hud delegates");

	auto currentMission = gameData->GetCurrentMission();
	MainObjective = FObjectiveItem(currentMission->GetMissionDetails()->GetMainObjective());

	if (hudDelegates->_UpdateMainObjectiveText.IsBound()) {
		UDebugMessages::LogDisplay(this, "_UpdateMainObjectiveText is bound");
		hudDelegates->_UpdateMainObjectiveText.Broadcast(MainObjective.GetObjectiveText());
	}
	else
		GetWorld()->GetTimerManager().SetTimer(MainDelayerHandle, this, &UObjectivesManager::MainDelayerHandleCallback, 1.0f, false);
}

void UObjectivesManager::UpdateMainObjective(FString obj) {
	auto gameData = USvUtilities::GetCurrentGameData(GetWorld());
	if (!gameData)
		return UDebugMessages::LogError(this, "failed to get game data");

	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "failed to get hud delegates");

	if (obj.Len() == 0) {
		auto currentMission = gameData->GetCurrentMission();
		MainObjective.UpdateObjectiveText(currentMission->GetMissionDetails()->GetMainObjective());
	}
	else {
		MainObjective.UpdateObjectiveText(obj);
	}


	hudDelegates->_UpdateMainObjectiveText.Broadcast(MainObjective.GetObjectiveText());
}

void UObjectivesManager::MainDelayerHandleCallback() {
	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "failed to get hud delegates");

	if (hudDelegates->_UpdateMainObjectiveText.IsBound())
		hudDelegates->_UpdateMainObjectiveText.Broadcast(MainObjective.GetObjectiveText());
	else
		GetWorld()->GetTimerManager().SetTimer(MainDelayerHandle, this, &UObjectivesManager::MainDelayerHandleCallback, 1.0f, false);
}