// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAIController.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Components/GridMovementComponent.h"
#include "../../Interfaces/Movable.h"

void ACharacterAIController::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result) {
	UDebugMessages::LogDisplay(this, "move completed");

	if (auto pawn = Cast<IMovable>(GetPawn())) {
		auto component = pawn->GetGridMovementComponent();
		component->MovementLoop();
	}
}