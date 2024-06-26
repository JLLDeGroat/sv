// Fill out your copyright notice in the Description page of Project Settings.


#include "RightClickAction.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Interfaces/Movable.h"
#include "../../Interfaces/Selectable.h"
#include "../Managers/SelectionManager.h"
#include "../Components/PawnCameraComponent.h"

#include "../../Characters/Components/CharacterDetailsComponent.h"
#include "../../Characters/Components/GridMovementComponent.h"


// Sets default values for this component's properties
URightClickAction::URightClickAction(const FObjectInitializer& ObjectInitializer) : UBaseActionComponent(ObjectInitializer) 
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	ValidCameraStates.Emplace(ECameraState::CS_Control);
}


// Called when the game starts
void URightClickAction::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void URightClickAction::DoAction() {
	auto owner = GetOwner<AGamePlayerController>();
	auto pawn = owner->GetPawn();
	auto pawnCameraComponent = pawn->GetComponentByClass<UPawnCameraComponent>();

	if (!IsInValidCameraState(pawnCameraComponent->GetCurrentCameraState()))
		return;

	auto selected = SelectionManager->GetSelected();

	if (selected) {
		TScriptInterface<IMovable> movable;
		if (selected->TryGetAsMoveable(movable)) {
			auto selectedDetails = selected->GetAsActor()->GetComponentByClass<UCharacterDetailsComponent>();
			if (!selectedDetails || selectedDetails->GetMovementPoints() == 0) {
				UDebugMessages::LogError(this, "failed to get selected details or had zero movement points cannot move");
				return;
			}

			auto selectedMouseLocation = SelectionManager->GetCurrentMousedLocation();
			auto currentActorGridLocation = selected->GetSelectableGridLocation();
			//move the z axis lower to hit fence items
			currentActorGridLocation.Z -= 50;

			UDebugMessages::LogDisplay(this, "Moving from " + currentActorGridLocation.ToString() + " to: " + selectedMouseLocation.ToString());
			//testing grid system
			auto gridSteps = movable->GetGridMovementComponent()->FindRoute(currentActorGridLocation, selectedMouseLocation);
			//

			int stepsTaken = gridSteps.Num();

			selectedDetails->RemoveMovementPoints(stepsTaken);

			UDebugMessages::LogDisplay(this, "moving");
			movable->GetGridMovementComponent()->MoveAcrossGrid(SelectionManager->GetLocationPath());
		}
	}
}