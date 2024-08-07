// Fill out your copyright notice in the Description page of Project Settings.


#include "PostMovementRunnable.h"
#include "../Utilities/SvUtilities.h"
#include "../Utilities/GridUtilities.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Environment/Components/VaultableComponent.h"
#include "../Characters/Components/GridMovementComponent.h"
#include "../Characters/Components/CharacterDetailsComponent.h"
#include "../Characters/Components/AIComponent.h"
#include "../Characters/Components/ActionsComponent.h"
#include "../Delegates/HudDelegates.h"
#include "../Delegates/AIDelegates.h"
#include "DrawDebugHelpers.h"

void UPostMovementRunnable::ActivateThread() {

	auto gridLocation = UGridUtilities::GetNormalisedGridLocation(MovedActor->GetActorLocation());
	gridLocation.Z -= 50;
	TArray<FVector> adjacentLocations;
	USvUtilities::GetAdjacentGridTiles(gridLocation, adjacentLocations);

	for (int i = 0; i < adjacentLocations.Num(); i++) {
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, gridLocation, adjacentLocations[i], USvUtilities::GetEnvironmentChannel());
		//DrawDebugLine(GetWorld(), gridLocation, adjacentLocations[i], FColor::Red, true, 60, 0, 5);

		if (Hit.GetActor()) {
			UDebugMessages::LogDisplay(this, "found actor " + Hit.GetActor()->GetName() + " comp " + Hit.GetComponent()->GetName());

			if (Hit.GetActor()->GetComponentByClass<UVaultableComponent>()) {
				auto movementComponent = MovedActor->GetComponentByClass<UGridMovementComponent>();

				if (movementComponent)
					movementComponent->PostMovementCrouch();
			}
		}
	}

	auto details = MovedActor->GetComponentByClass<UCharacterDetailsComponent>();
	if (details && details->GetCharacterControl() == ECharacterControl::CC_Player) {
		auto hudDelegates = UHudDelegates::GetInstance();
		if (!hudDelegates)
			return UDebugMessages::LogError(this, "failed to get hudDelegates");

		auto actionsComponent = MovedActor->GetComponentByClass<UActionsComponent>();

		if (!actionsComponent)
			return UDebugMessages::LogError(this, "failed to get actions component");

		FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([hudDelegates, actionsComponent] {
			actionsComponent->SendActionsToUI();
			hudDelegates->_RefreshCharacterDetailsWidget.Broadcast();
			}, TStatId(), nullptr, ENamedThreads::GameThread);
	}

	if (MovedActor->GetComponentByClass<UAIComponent>()) {
		auto aiDelegates = UAIDelegates::GetInstance();
		if (!aiDelegates)
			return UDebugMessages::LogError(this, "failed to get ai delegates");

		FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([aiDelegates] {
			aiDelegates->_AICharacterFinishedBehaviour.Broadcast();
			}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}

UPostMovementRunnable* UPostMovementRunnable::InsertVariables(AActor* movementActor) {
	MovedActor = movementActor;
	return this;
}