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

#include "VgCore/Domain/Debug/DebugMessages.h"
UAIMeleeRangeMove::UAIMeleeRangeMove(const FObjectInitializer& ObjectInitializer)
	:UBaseAIBehaviour(ObjectInitializer) {

}

void UAIMeleeRangeMove::DoBehaviour() {
	auto closestCharacter = GetAllCharacters()[0];

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
	for (int k = 0; k < finalPossibleDestinationLocs.Num(); k++) {
		if (AttemptToRouteToPossibleLocation(GetThisEnemy(), finalPossibleDestinationLocs[k])) {
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
	//auto movementDetails = gridMovementComponent->FindQuickestRoute(sourceLoc, possibleLocation, true);

	TArray<FVector> locs;
	FindPathPointsToLocation(sourceLoc, possibleLocation, locs);

	SpawnDebugGrid_SetIsEnd(locs, FVector::ZeroVector, 1.0f);

	auto thisEnemyDetails = character->GetAsActor()->GetComponentByClass<UCharacterDetailsComponent>();
	auto maximumMovementDistance = thisEnemyDetails->GetMovementPoints() * 100;

	int totalDistance = 0;
	FVector locationToMoveTo = FVector::ZeroVector;
	for (int i = 0; i < locs.Num(); i++) {
		auto thisDistance = FVector::Dist(locs[i], sourceLoc);

		auto newTotalDistance = thisDistance + totalDistance;
		if (newTotalDistance > maximumMovementDistance) {
			auto leftOverDistance = newTotalDistance - maximumMovementDistance;
			locationToMoveTo = GetPointBetweenVectors(locationToMoveTo, locs[i], thisDistance - leftOverDistance);
			break;
		}
		else {
			totalDistance = newTotalDistance;
			locationToMoveTo = locs[i];
		}
	}

	SpawnDebugGrid_SetIsEnd(locationToMoveTo, 1.0f);

	auto gridLoc = UGridUtilities::GetNormalisedGridLocation(locationToMoveTo);
	TArray<FVector> finalLocations;

	auto foundRoute = gridMovementComponent->FindRoute(sourceLoc, gridLoc, true);

	if (!foundRoute.IsEmpty())
		finalLocations = foundRoute;
	else {
		TArray<FVector> adjacentLocs;
		USvUtilities::GetAdjacentGridTiles(gridLoc, adjacentLocs);

		for (int i = 0; i < adjacentLocs.Num(); i++) {
			foundRoute = gridMovementComponent->FindRoute(sourceLoc, adjacentLocs[i], true);
			if (!foundRoute.IsEmpty()) {
				UDebugMessages::LogWarning(this, "had to go with a second choice for movement");
				finalLocations = foundRoute;
				break;
			}
		}
	}

	SpawnDebugGrid_SetIsObstacle(finalLocations, FVector::ZeroVector, 1.0f);

	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([finalLocations, gridMovementComponent]
		{
			gridMovementComponent->MoveAcrossGrid(finalLocations);
		},
		TStatId(), nullptr, ENamedThreads::GameThread);

	return !finalLocations.IsEmpty();
}

//SpawnDebugGrid_SetIsObstacle(movementDetails);


//TArray<FVector> finalMovement;
//if (!movementDetails.IsEmpty()) {
//	auto thisEnemyDetails = character->GetAsActor()->GetComponentByClass<UCharacterDetailsComponent>();

//	if (movementDetails.Num() < thisEnemyDetails->GetMovementPoints()) {
//		finalMovement = movementDetails;
//	}
//	else {
//		for (int j = 0; j < thisEnemyDetails->GetMovementPoints(); j++)
//			finalMovement.Emplace(movementDetails[j]);
//	}
//}

//if (!finalMovement.IsEmpty()) {
//	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([finalMovement, gridMovementComponent]
//		{
//			gridMovementComponent->MoveAcrossGrid(finalMovement);
//		},
//		TStatId(), nullptr, ENamedThreads::GameThread);
//	return true;
//}

void UAIMeleeRangeMove::FindPathPointsToLocation(FVector start, FVector end, TArray<FVector>& navPath) {
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSys)
	{
		// Get the current AI character's location
		//FVector StartLocation = GetPawn()->GetActorLocation();

		// Calculate the path
		UNavigationPath* NavPath = NavSys->FindPathToLocationSynchronously(GetWorld(), start, end);

		if (NavPath && NavPath->IsValid())
		{
			// Iterate through the path points
			for (const FVector& PathPoint : NavPath->PathPoints)
			{
				UE_LOG(LogTemp, Log, TEXT("Path Point: %s"), *PathPoint.ToString());

				// Optional: Visualize the path points in the world
				//DrawDebugSphere(GetWorld(), PathPoint, 25.0f, 12, FColor::Green, false, 5.0f);
			}

			// Optionally, if you want to execute the move after displaying the path
			//MoveToLocation(Destination);
			navPath = NavPath->PathPoints;
			NavPath->ClearInternalFlags(EInternalObjectFlags::Async);
		}
	}
}

FVector UAIMeleeRangeMove::GetPointBetweenVectors(FVector StartVector, FVector EndVector, float DistanceFromStart)
{
	// Ensure that the distance between the vectors is more than 0
	if (DistanceFromStart <= 0.0f)
	{
		return StartVector; // Return the start vector if distance is 0 or negative
	}

	// Calculate the total distance between the two vectors
	float TotalDistance = FVector::Dist(StartVector, EndVector);

	// Clamp the distance to ensure it doesn't exceed the total distance
	DistanceFromStart = FMath::Clamp(DistanceFromStart, 0.0f, TotalDistance);

	// Calculate the direction vector from the start vector to the end vector
	FVector DirectionVector = EndVector - StartVector;

	// Normalize the direction vector to get a unit vector
	DirectionVector.Normalize();

	// Calculate the target vector by moving along the direction vector by the specified distance
	FVector TargetVector = StartVector + DirectionVector * DistanceFromStart;

	return TargetVector;
}
