// Fill out your copyright notice in the Description page of Project Settings.


#include "AiRangeMove.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

#include "Components/AiMovementComponent.h"
#include "../../../Utilities/SvUtilities.h"
#include "../../../Utilities/GridUtilities.h"

#include "../../../Interfaces/SvChar.h"

#include "../../../Characters/Components/CharacterDetailsComponent.h"
#include "../../../Characters/Components/TargetingComponent.h"
#include "../../../Characters/Components/GridMovementComponent.h"

#include "../../../Player/Components/PawnCameraComponent.h"

UAiRangeMove::UAiRangeMove(const FObjectInitializer& ObjectInitializer)
	:UBaseAIBehaviour(ObjectInitializer) {

}
#pragma optimize("", off)
void UAiRangeMove::DoBehaviour() {
	UDebugMessages::LogError(this, "no range movement coded");

	if (CheckPositionIsGoodForRange(GetThisEnemy()->GetActorLocation())) {
		return SetCompletedBehaviour();
	}

	auto closestEnemy = GetClosestCharacter();

	auto details = GetThisEnemy()->GetComponentByClass<UCharacterDetailsComponent>();
	auto movementAmount = details->GetMovementPoints();

	auto locationsInRadius = UGridUtilities::GetAllLocationsWithinRadius(
		UGridUtilities::GetNormalisedGridLocation(GetThisEnemy()->GetActorLocation()),
		movementAmount);

	auto currentLocation = GetThisEnemy()->GetActorLocation();

	Algo::Sort(locationsInRadius, [&currentLocation](const FVector& A, const FVector& B)
		{
			// Calculate squared distance to avoid unnecessary square root operations
			float DistanceA = FVector::DistSquared(A, currentLocation);
			float DistanceB = FVector::DistSquared(B, currentLocation);

			// Return true if A is closer to the ReferencePoint than B
			return DistanceA < DistanceB;
		});


	auto movementComponent = GetMovementComponent();

	bool foundGoodPosition = false;
	for (int i = 0; i < locationsInRadius.Num(); i++) {
		if (CheckPositionIsGoodForRange(locationsInRadius[i])) {

			TArray<FVector> movement;
			if (movementComponent->AttemptToRouteToPossibleLocation(GetThisEnemy(), locationsInRadius[i], movement)) {
				foundGoodPosition = true;

				if (!GetIsInFog()) {
					auto pawnCamera = GetWorld()->GetFirstPlayerController()->GetPawn()->GetComponentByClass<UPawnCameraComponent>();
					pawnCamera->UpdateCameraState(ECameraState::CS_ReTarget, GetThisEnemy()->GetActorLocation());
				}

				auto gridMovementComponent = GetThisEnemy()->GetComponentByClass<UGridMovementComponent>();
				FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([movement, gridMovementComponent]
					{
						gridMovementComponent->MoveAcrossGrid(movement);
					},
					TStatId(), nullptr, ENamedThreads::GameThread);
				break;
			}
		}
	}

	if (!foundGoodPosition)
		SetCompletedBehaviour();
}

bool UAiRangeMove::CheckPositionIsGoodForRange(FVector loc) {
	auto closestEnemy = GetClosestCharacter();

	auto currentGridLocation = UGridUtilities::GetNormalisedGridLocation(loc);

	auto targetingComponent = GetThisEnemy()->GetComponentByClass<UTargetingComponent>();

	targetingComponent->DetermineTheoreticalTargetData(currentGridLocation);
	auto targetingData = targetingComponent->GetCurrentTheoreticalTargetData();

	for (int i = 0; i < targetingData.Num(); i++) {
		if (targetingData[i].GetCharacter() == closestEnemy &&
			FVector::Dist(targetingData[i].GetShootLocation(), currentGridLocation) > 75)
		{
			// assuming we have to move to shoot therefore there is no current main line of sight
			return true;
		}
	}

	return false;
}
#pragma optimize("", on)