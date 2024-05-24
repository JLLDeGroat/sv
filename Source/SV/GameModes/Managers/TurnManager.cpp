// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnManager.h"
#include "../../Player/GamePlayerController.h"
#include "../../Interfaces/SvChar.h"
#include "../../Runnables/AITurnRunnable.h"
#include "../../Characters/Components/CharacterDetailsComponent.h"
#include "../GameplayMode.h"
#include "CharacterManager.h"

UTurnManager::UTurnManager(const FObjectInitializer& ObjectInitializer) : UObject(ObjectInitializer) {

}

void UTurnManager::BeginAITurn() {
	NewObject<UAITurnRunnable>()
		->Initialise(GetWorld())
		->Begin();
}

void UTurnManager::BeginPlayerTurn() {
	auto gameplayMode = GetWorld()->GetAuthGameMode<AGameplayMode>();
	if (!gameplayMode) {
		UDebugMessages::LogDisplay(this, "failed to get gameplay to start player turn");
		return;
	}
	auto characterManager = gameplayMode->GetCharacterManager();
	if (!characterManager) {
		UDebugMessages::LogDisplay(this, "failed to get characterManager to start player turn");
		return;
	}

	TArray<TScriptInterface<ISvChar>> foundCharacters;
	characterManager->GetCharacterListOfCharacterType(ECharacterControl::CC_Player, foundCharacters);

	for (int i = 0; i < foundCharacters.Num(); i++) {
		if (!foundCharacters[i]) continue;

		auto characterActor = foundCharacters[i]->GetAsActor();
		auto detailsComponent = characterActor->GetComponentByClass<UCharacterDetailsComponent>();
		
		if (!detailsComponent) 
			return UDebugMessages::LogError(this, "failed to get details component, cannot begin player turn");

		detailsComponent->RefreshOnNewTurn();
	}
}