// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtractionAction.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

#include "../../Characters/Components/CharacterDetailsComponent.h"
#include "../../Characters/Components/ThrowableComponent.h"
#include "../../Characters/Components/DestroyComponent.h"

#include "../Managers/SelectionManager.h"
#include "../../Interfaces/Selectable.h"
#include "../../Interfaces/SvChar.h"

#include "../../Instance/Managers/CurrentGameDataManager.h"
#include "../../Instance/SvGameInstance.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Utilities/GridUtilities.h"

#include "../../Runnables/Checkers/WinLossCheckerRunnable.h"
#include "../../GameModes/Managers/CharacterManager.h"

UExtractionAction::UExtractionAction(const FObjectInitializer& ObjectInitializer)
	:UBaseActionComponent(ObjectInitializer) {

}
#pragma optimize("", off)
void UExtractionAction::DoAction() {
	ResetActionEffects();

	auto selected = SelectionManager->GetSelected();
	if (selected) {
		auto thisAsSvChar = (ISvChar*)selected->GetAsActor();
		auto thisAsActor = selected->GetAsActor();


		auto instance = USvUtilities::GetGameInstance(GetWorld());
		if (!instance)
			return UDebugMessages::LogError(this, "failed to get game instance");

		auto characterDetails = thisAsActor->GetComponentByClass<UCharacterDetailsComponent>();

		if (!characterDetails)
			return UDebugMessages::LogError(this, "failed to get character details");

		auto currentGameData = instance->GetCurrentGameDataManager()->GetCurrentGameData();

		auto crew = currentGameData->GetCrewMember(characterDetails->GetCharacterId());
		crew->SetHealth(characterDetails->GetHealth());

		auto throwableComponent = thisAsActor->GetComponentByClass<UThrowableComponent>();
		if (throwableComponent) {
			auto previouslyThrown = throwableComponent->GetPreviouslyThrownThrowables();

			for (int i = 0; i < previouslyThrown.Num(); i++)
				currentGameData->RemoveTool(previouslyThrown[i].GetThrowableId());
		}

		auto destroyComponent = thisAsActor->GetComponentByClass<UDestroyComponent>();
		if (!destroyComponent)
			return UDebugMessages::LogError(this, thisAsActor->GetName() + " Does not have destruction component");

		destroyComponent->DestroyThisActor();

		auto characterManager = USvUtilities::GetGameModeCharacterManager(GetWorld());
		if (!characterManager)
			return UDebugMessages::LogError(this, "failed to get character manager");
		
		characterManager->AddToExtractedCharacterList(selected->GetAsActor());
		characterManager->RemoveCharacter(characterDetails->GetCharacterId());

		BaseRunnable = NewObject<UWinLossCheckerRunnable>()
			->Initialise(GetWorld())
			->Begin();
	}
}
#pragma optimize("", on)