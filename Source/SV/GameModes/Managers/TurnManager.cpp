// Fill out your copyright notice in the Description page of Project Settings.

#include "TurnManager.h"
#include "../../Player/GamePlayerController.h"
#include "../../Interfaces/SvChar.h"
#include "../../Runnables/AITurnRunnable.h"
#include "../../Characters/Components/CharacterDetailsComponent.h"
#include "../../Delegates/GameplayDelegates.h"
#include "../../Delegates/AIDelegates.h"
#include "../../Delegates/HudDelegates.h"
#include "../../Utilities/SvUtilities.h"
#include "../GameplayMode.h"
#include "../../Data/Game/FCurrentGameData.h"
#include "ObjectivesManager.h"
#include "CharacterManager.h"

UTurnManager::UTurnManager(const FObjectInitializer &ObjectInitializer) : UObject(ObjectInitializer)
{
}

void UTurnManager::BeginAITurn()
{
	auto gameplayDelegates = UGameplayDelegates::GetInstance();
	if (!gameplayDelegates)
		return UDebugMessages::LogError(this, "failed to get gameplay delegates");

	auto aiDelegates = UAIDelegates::GetInstance();
	if (!aiDelegates)
		return UDebugMessages::LogError(this, "failed to get ai delegates");

	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "failed to get hud delegates");

	gameplayDelegates->_ActivateOverwatchActors.Broadcast(ECharacterControl::CC_Player);
	gameplayDelegates->_RemoveUnusedOverwatchActors.Broadcast(ECharacterControl::CC_AI);

	hudDelegates->_CharacterTileVisibility.Broadcast(false);
	hudDelegates->_ResetCharacterTileWidget.Broadcast();
	hudDelegates->_HideOrResetUIWidget.Broadcast();

	aiDelegates->_AiTurnIndicatorVisibility.Broadcast(true);

	Runnable = NewObject<UAITurnRunnable>(this)
				   ->Initialise(GetWorld())
				   ->Begin();
}

void UTurnManager::BeginPlayerTurn()
{
	if (!this || !GetWorld())
		return;

	auto currentGameData = USvUtilities::GetCurrentGameData(GetWorld());
	if (!currentGameData)
		return UDebugMessages::LogError(this, "failed to get currentGameData");

	auto gameplayMode = GetWorld()->GetAuthGameMode<AGameplayMode>();
	if (!gameplayMode)
	{
		UDebugMessages::LogDisplay(this, "failed to get gameplay to start player turn");
		return;
	}
	auto characterManager = gameplayMode->GetCharacterManager();
	if (!characterManager)
	{
		UDebugMessages::LogDisplay(this, "failed to get characterManager to start player turn");
		return;
	}
	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "failed to get hud delegates");

	auto gameplayDelegates = UGameplayDelegates::GetInstance();
	if (!gameplayDelegates)
		return UDebugMessages::LogError(this, "failed to get gameplay delegates");

	auto aiDelegates = UAIDelegates::GetInstance();
	if (!aiDelegates)
		return UDebugMessages::LogError(this, "failed to get ai delegates");

	auto currentMission = currentGameData->GetCurrentMission()->GetMissionDetails();
	currentMission->AddToTurnCounter();
	UDebugMessages::LogDisplay(this, "current turn " + FString::SanitizeFloat(currentMission->GetTurn()));

	// mission complete, end here
	if (currentMission->GetTurn() >= currentMission->GetTurnLimit())
		return hudDelegates->_ShowMissionCompleteWidget.Broadcast();

	auto objectivesManager = gameplayMode->GetObjectivesManager();
	objectivesManager->UpdateMainObjective();

	hudDelegates->_CharacterTileVisibility.Broadcast(true);
	aiDelegates->_AiTurnIndicatorVisibility.Broadcast(false);

	UDebugMessages::LogError(this, "TODO current activations are commented out");
	gameplayDelegates->_ActivateOverwatchActors.Broadcast(ECharacterControl::CC_AI);
	gameplayDelegates->_RemoveUnusedOverwatchActors.Broadcast(ECharacterControl::CC_Player);

	TArray<TScriptInterface<ISvChar>> foundCharacters;
	characterManager->GetCharacterListOfCharacterType(ECharacterControl::CC_Player, foundCharacters);

	for (int i = 0; i < foundCharacters.Num(); i++)
	{
		if (!foundCharacters[i])
			continue;

		auto characterActor = foundCharacters[i]->GetAsActor();
		auto detailsComponent = characterActor->GetComponentByClass<UCharacterDetailsComponent>();

		if (!detailsComponent)
			return UDebugMessages::LogError(this, "failed to get details component, cannot begin player turn");

		detailsComponent->RefreshOnNewTurn();
	}
}

void UTurnManager::KillRunnable()
{
	if (this && Runnable)
	{
		auto turnRunnable = (UAITurnRunnable *)Runnable;
		if (turnRunnable)
		{
			turnRunnable->KillThreads();
			turnRunnable->EnsureCompletion();
		}
	}
}

int UTurnManager::GetCurrentTurn()
{
	auto currentGameData = USvUtilities::GetCurrentGameData(GetWorld());
	auto currentMission = currentGameData->GetCurrentMission()->GetMissionDetails();
	return currentMission->GetTurn();
}