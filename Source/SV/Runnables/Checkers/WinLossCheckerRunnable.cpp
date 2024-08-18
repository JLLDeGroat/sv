// Fill out your copyright notice in the Description page of Project Settings.


#include "WinLossCheckerRunnable.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Instance/SvGameInstance.h"
#include "../../Instance/Managers/CurrentGameDataManager.h"
#include "../../GameModes/GameplayMode.h"
#include "../../GameModes/Managers/CharacterManager.h"
#include "../../GameModes/Managers/WinLossManager.h"
#include "../../Delegates/HudDelegates.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

#pragma optimize("", off)
void UWinLossCheckerRunnable::ActivateThread() {
	UDebugMessages::LogDisplay(this, "checking winloss");

	auto winLossManager = USvUtilities::GetGameModeWinLossManager(GetWorld());
	if (!winLossManager)
		return UDebugMessages::LogError(this, "failed to get win loss manager");

	auto characterManager = USvUtilities::GetGameModeCharacterManager(GetWorld());
	if (!characterManager)
		return UDebugMessages::LogError(this, "failed to get character manager");

	auto instance = USvUtilities::GetGameInstance(GetWorld());
	if (!instance || !instance->GetCurrentGameDataManager() || !instance->GetCurrentGameDataManager()->GetCurrentGameData())
		return UDebugMessages::LogError(this, "failed to get current game data");

	auto currentGameData = instance->GetCurrentGameDataManager()->GetCurrentGameData();

	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "failed to get hud delegates");


	if (GetHasAllCharactersLeftTheField()) {
		if (GetHasExtractedAnyCharacters()) {
			SetHasCompletedThisMission();
			FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([hudDelegates] {
				hudDelegates->_ShowMissionCompleteWidget.Broadcast();
				}, TStatId(), nullptr, ENamedThreads::GameThread);
		}
		else {
			FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([hudDelegates] {
				hudDelegates->_ShowMissionFailedWidget.Broadcast();
				}, TStatId(), nullptr, ENamedThreads::GameThread);
		}
	}
	else {
		auto missionType = currentGameData->GetCurrentMission()->GetType();

		switch (missionType) {
		case EMissionType::MT_Exterminate:
		{
			if (GetHasExterminationComplete()) {
				SetHasCompletedThisMission();
				FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([hudDelegates] {
					hudDelegates->_ShowMissionCompleteWidget.Broadcast();
					}, TStatId(), nullptr, ENamedThreads::GameThread);
				return;
			}
		}
		break;
		case EMissionType::MT_Survive:
		{

		}
		break;
		case EMissionType::MT_Traverse:
		{
			// this one onoly happens if everyone has extracted
		}
		break;
		case EMissionType::MT_Extraction:
		{
			// this one only happens if every one has extracted with something
		}
		break;
		default:
			return UDebugMessages::LogError(this, "failed to get valid missionType");
		}

	}
}

bool UWinLossCheckerRunnable::GetHasAllCharactersLeftTheField() {
	auto characterManager = USvUtilities::GetGameModeCharacterManager(GetWorld());

	TArray<TScriptInterface<ISvChar>> svChars;
	characterManager->GetCharacterListOfCharacterType(ECharacterControl::CC_Player, svChars);

	return svChars.IsEmpty();
}

bool UWinLossCheckerRunnable::GetHasExtractedAnyCharacters() {
	auto characterManager = USvUtilities::GetGameModeCharacterManager(GetWorld());
	return !characterManager->GetExtractedCharacters().IsEmpty();
}

bool UWinLossCheckerRunnable::GetHasExterminationComplete() {
	auto characterManager = USvUtilities::GetGameModeCharacterManager(GetWorld());

	TArray<TScriptInterface<ISvChar>> svChars;
	characterManager->GetCharacterListOfCharacterType(ECharacterControl::CC_AI, svChars);

	return svChars.IsEmpty();
}


void UWinLossCheckerRunnable::SetHasCompletedThisMission() {
	auto gameData = USvUtilities::GetCurrentGameData(GetWorld());

	if (!gameData)
		return UDebugMessages::LogError(this, "failed to get current game data");

	auto worldData = gameData->GetWorldData();
	auto currentMission = worldData->GetCurrentLocation()->GetMissionDetails();
	currentMission->SetIsCompleted(true);
}
#pragma optimize("", on)