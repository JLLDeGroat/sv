// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupAction.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Components/PawnCameraComponent.h"
#include "../../Interfaces/Selectable.h"
#include "../Managers/SelectionManager.h"

#include "../../Characters/Components/EquipmentComponent.h"
#include "../../Characters/Components/CharacterDetailsComponent.h"
#include "../../Characters/Components/PickupResourceComponent.h"
#include "../../Equipment/Components/EquipmentDetailsComponent.h"
#include "../../Equipment/Equipment.h"
#include "../../Environment/Pickups/Components/PickupIndicatorLink.h"
#include "../../Delegates/GameplayDelegates.h"

UPickupAction::UPickupAction(const FObjectInitializer& ObjectInitializer)
	: UBaseActionComponent(ObjectInitializer) {

	ValidCameraStates.Emplace(ECameraState::CS_Control);
}

void UPickupAction::BeginPlay() {
	Super::BeginPlay();
}
void UPickupAction::DoAction() {
	if (IsWithinValidControlLimiter()) {
		ResetActionEffects();

		auto controller = GetOwner<APlayerController>();
		auto pawn = controller->GetPawn();
		auto pawnCameraComponent = pawn->GetComponentByClass<UPawnCameraComponent>();

		if (!pawnCameraComponent)
			return UDebugMessages::LogError(this, "failed to get pawn camera component");

		if (!IsInValidCameraState(pawnCameraComponent->GetCurrentCameraState()))
			return;

		auto selected = SelectionManager->GetSelected();
		if (selected) {
			auto selectedAsActor = selected->GetAsActor();

			if (!selectedAsActor)
				return UDebugMessages::LogError(this, "failed to get selected as actor");

			auto pickupComponent = selectedAsActor->GetComponentByClass<UPickupResourceComponent>();

			if (!pickupComponent)
				return UDebugMessages::LogError(this, "failed to get pickup component");

			auto gameplayDelegates = UGameplayDelegates::GetInstance();
			if (!gameplayDelegates)
				return UDebugMessages::LogError(this, "failed to get gameplay delegats");

			gameplayDelegates->_ChangeControlLimits.Broadcast(EControlLimit::CL_NoClick);
			pickupComponent->StartPickup();
		}
	}
}