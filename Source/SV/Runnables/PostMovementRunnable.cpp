// Fill out your copyright notice in the Description page of Project Settings.


#include "PostMovementRunnable.h"
#include "../Utilities/SvUtilities.h"
#include "../Utilities/GridUtilities.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Environment/Components/VaultableComponent.h"
#include "../Characters/Components/GridMovementComponent.h"

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
}

UPostMovementRunnable* UPostMovementRunnable::InsertVariables(AActor* movementActor) {
	MovedActor = movementActor;
	return this;
}