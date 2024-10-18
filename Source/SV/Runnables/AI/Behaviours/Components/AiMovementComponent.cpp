// Fill out your copyright notice in the Description page of Project Settings.


#include "AiMovementComponent.h"
#include "../../../../Interfaces/SvChar.h"
#include "../../../../Utilities/GridUtilities.h"
#include "../../../../Utilities/SvUtilities.h"
#include "../../../../Characters/Components/GridMovementComponent.h"
#include "../../../../Characters/Components/CharacterDetailsComponent.h"

#include "VgCore/Domain/Debug/DebugMessages.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

#pragma optimize("", off)
bool UAiMovementComponent::AttemptToRouteToPossibleLocation(TScriptInterface<ISvChar> character, FVector possibleLocation, TArray<FVector>& finalLocations) {

	auto gridMovementComponent = character->GetAsActor()->GetComponentByClass<UGridMovementComponent>();
	if (!gridMovementComponent) {
		UDebugMessages::LogError(this, "Failed to get movement component, AI cannot move");
		return false;
	}

	auto sourceLoc = UGridUtilities::GetNormalisedGridLocation(character->GetAsActor()->GetActorLocation());
	//auto movementDetails = gridMovementComponent->FindQuickestRoute(sourceLoc, possibleLocation, true);

	TArray<FVector> locs;
	FindPathPointsToLocation(sourceLoc, possibleLocation, locs);

	//SpawnDebugGrid_SetIsEnd(locs, FVector::ZeroVector, 1.0f);

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

	//SpawnDebugGrid_SetIsEnd(locationToMoveTo, 1.0f);

	auto gridLoc = UGridUtilities::GetNormalisedGridLocation(locationToMoveTo);
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

	//SpawnDebugGrid_SetIsObstacle(finalLocations, FVector::ZeroVector, 1.0f);

	/*FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([finalLocations, gridMovementComponent]
		{
			gridMovementComponent->MoveAcrossGrid(finalLocations);
		},
		TStatId(), nullptr, ENamedThreads::GameThread);*/

	return !finalLocations.IsEmpty();
}

void UAiMovementComponent::FindPathPointsToLocation(FVector start, FVector end, TArray<FVector>& navPath) {
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSys)
	{
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

			navPath = NavPath->PathPoints;
			NavPath->ClearInternalFlags(EInternalObjectFlags::Async);
		}
	}
}

FVector UAiMovementComponent::GetPointBetweenVectors(FVector StartVector, FVector EndVector, float DistanceFromStart) {
	// Ensure that the distance between the vectors is more than 0
	if (DistanceFromStart <= 0.0f)
	{
		return StartVector; // Return the start vector if distance is 0 or negative
	}

	float TotalDistance = FVector::Dist(StartVector, EndVector);

	DistanceFromStart = FMath::Clamp(DistanceFromStart, 0.0f, TotalDistance);

	FVector DirectionVector = EndVector - StartVector;
	
	DirectionVector.Normalize();
	FVector TargetVector = StartVector + DirectionVector * DistanceFromStart;

	return TargetVector;
}
#pragma optimize("", on)