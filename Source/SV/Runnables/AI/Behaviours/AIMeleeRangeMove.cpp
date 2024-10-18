// Fill out your copyright notice in the Description page of Project Settings.


#include "AIMeleeRangeMove.h"

#include "../../../Interfaces/SvChar.h"
#include "../../../Utilities/GridUtilities.h"
#include "../../../Utilities/SvUtilities.h"

#include "../../../Characters/Components/GridMovementComponent.h"
#include "../../../Characters/Components/CharacterDetailsComponent.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

#include "Components/AiMovementComponent.h"

#include "VgCore/Domain/Debug/DebugMessages.h"

UAIMeleeRangeMove::UAIMeleeRangeMove(const FObjectInitializer& ObjectInitializer)
	:UBaseAIBehaviour(ObjectInitializer) {

}

void UAIMeleeRangeMove::DoBehaviour() {
	auto closestCharacter = GetClosestCharacter();

	auto sourceLoc = UGridUtilities::GetNormalisedGridLocation(GetThisEnemy()->GetActorLocation());

	TArray<FVector> possibleDestinationLocs;
	TArray<FVector> finalPossibleDestinationLocs;
	auto characterNormalisedLocation = UGridUtilities::GetNormalisedGridLocation(closestCharacter->GetAsActor()->GetActorLocation());
	USvUtilities::GetAdjacentGridTiles(UGridUtilities::GetNormalisedGridLocation(closestCharacter->GetAsActor()->GetActorLocation()), possibleDestinationLocs);

	//check if one of the directions has melee capabilities
	for (int i = 0; i < possibleDestinationLocs.Num(); i++) {
		FHitResult EnvironmentHit;
		GetWorld()->LineTraceSingleByChannel(EnvironmentHit, possibleDestinationLocs[i], characterNormalisedLocation, USvUtilities::GetEnvironmentChannel());
		if (!EnvironmentHit.bBlockingHit) {
			finalPossibleDestinationLocs.Emplace(possibleDestinationLocs[i]);
		}
	}

	finalPossibleDestinationLocs = USvUtilities::OrderByClosestTo(sourceLoc, finalPossibleDestinationLocs);

	auto foundRoute = false;
	TArray<FVector> finalLocations;
	for (int k = 0; k < finalPossibleDestinationLocs.Num(); k++) {
		if (GetMovementComponent()->AttemptToRouteToPossibleLocation(GetThisEnemy(), finalPossibleDestinationLocs[k], finalLocations)) {
			foundRoute = true;
			break;
		}
	}

	auto gridMovementComponent = GetThisEnemy()->GetComponentByClass<UGridMovementComponent>();
	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([finalLocations, gridMovementComponent]
		{
			gridMovementComponent->MoveAcrossGrid(finalLocations);
		},
		TStatId(), nullptr, ENamedThreads::GameThread);


	if (!foundRoute) {
		UDebugMessages::LogError(this, "was unable to move");
		SetCompletedBehaviour();
	}
}