// Fill out your copyright notice in the Description page of Project Settings.


#include "AIMeleeRangeMove.h"

#include "../../../Interfaces/SvChar.h"
#include "../../../Utilities/GridUtilities.h"
#include "../../../Utilities/SvUtilities.h"

#include "../../../Characters/Components/GridMovementComponent.h"
#include "../../../Characters/Components/CharacterDetailsComponent.h"

#include "VgCore/Domain/Debug/DebugMessages.h"
UAIMeleeRangeMove::UAIMeleeRangeMove(const FObjectInitializer& ObjectInitializer)
	:UBaseAIBehaviour(ObjectInitializer) {

}

void UAIMeleeRangeMove::DoBehaviour() {
	auto closestCharacter = GetAllCharacters()[0];

	auto sourceLoc = UGridUtilities::GetNormalisedGridLocation(GetThisEnemy()->GetActorLocation());

	TArray<FVector> possibleDestinationLocs;
	USvUtilities::GetAdjacentGridTiles(UGridUtilities::GetNormalisedGridLocation(closestCharacter->GetAsActor()->GetActorLocation()), possibleDestinationLocs);

	possibleDestinationLocs = USvUtilities::OrderByClosestTo(sourceLoc, possibleDestinationLocs);

	auto foundRoute = false;
	for (int k = 0; k < possibleDestinationLocs.Num(); k++) {
		if (AttemptToRouteToPossibleLocation(GetThisEnemy(), possibleDestinationLocs[k])) {
			foundRoute = true;
			break;
		}
	}

	if (!foundRoute) {
		UDebugMessages::LogError(this, "was unable to move");
		SetCompletedBehaviour();
	}
}

bool UAIMeleeRangeMove::AttemptToRouteToPossibleLocation(TScriptInterface<ISvChar> character, FVector possibleLocation) {

	auto gridMovementComponent = character->GetAsActor()->GetComponentByClass<UGridMovementComponent>();
	if (!gridMovementComponent) {
		UDebugMessages::LogError(this, "Failed to get movement component, AI cannot move");
		return false;
	}

	auto sourceLoc = UGridUtilities::GetNormalisedGridLocation(character->GetAsActor()->GetActorLocation());
	auto movementDetails = gridMovementComponent->FindRoute(sourceLoc, possibleLocation, true);

	TArray<FVector> finalMovement;
	if (!movementDetails.IsEmpty()) {
		auto thisEnemyDetails = character->GetAsActor()->GetComponentByClass<UCharacterDetailsComponent>();

		if (movementDetails.Num() < thisEnemyDetails->GetMovementPoints()) {
			finalMovement = movementDetails;
		}
		else {
			for (int j = 0; j < thisEnemyDetails->GetMovementPoints(); j++)
				finalMovement.Emplace(movementDetails[j]);
		}
	}

	if (!finalMovement.IsEmpty()) {
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([finalMovement, gridMovementComponent]
			{
				gridMovementComponent->MoveAcrossGrid(finalMovement);
			},
			TStatId(), nullptr, ENamedThreads::GameThread);
		return true;
	}

	return false;
}