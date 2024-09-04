// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtractionChecker.h"
#include "../../../Interfaces/SvChar.h"
#include "../../../Characters/Components/InventoryComponent.h"
#include "../../../GameModes/Managers/ObjectivesManager.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../Utilities/SvUtilities.h"

UExtractionChecker::UExtractionChecker() {

}

UBaseMissionTypeWLChecker* UExtractionChecker::ActivateChecker() {

	auto characters = GetAllCharactersLeftOnTheField();

	bool hasIntel = false;

	for (int i = 0; i < characters.Num(); i++) {
		if (characters[i]) {
			auto asActor = characters[i]->GetAsActor();
			auto inventoryComponent = asActor->GetComponentByClass<UInventoryComponent>();
			if (inventoryComponent && inventoryComponent->GetIntel() > 0) {
				hasIntel = true;
				break;
			}
		}
	}

	auto objectivesManager = USvUtilities::GetGameModeObjectiveManager(GetWorld());

	if (hasIntel) {
		FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([objectivesManager] {
			objectivesManager->UpdateMainObjective("Get to Extraction");
			}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
	else {
		FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([objectivesManager] {
			objectivesManager->UpdateMainObjective();
			}, TStatId(), nullptr, ENamedThreads::GameThread);
	}

	return this;
}