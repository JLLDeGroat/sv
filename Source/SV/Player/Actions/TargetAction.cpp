// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetAction.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Enums/ECharacterEnums.h"
#include "../Managers/SelectionManager.h"
#include "../../Utilities/SvUtilities.h"
#include "../Components/PawnCameraComponent.h"
#include "Camera/CameraComponent.h"
#include "../../Characters/Components/EquipmentComponent.h"
#include "../../Characters/Components/TargetingComponent.h"
#include "../../Characters/Components/CharacterDetailsComponent.h"
#include "../../Characters/Components/AttackComponent.h"
#include "../../Interfaces/SvChar.h"
#include "../../Interfaces/Selectable.h"
#include "../GamePlayerController.h"
#include "../../Delegates/HudDelegates.h"

UTargetAction::UTargetAction(const FObjectInitializer& ObjectInitializer) :UBaseActionComponent(ObjectInitializer) {
	ValidCameraStates.Emplace(ECameraState::CS_Control);
	ValidCameraStates.Emplace(ECameraState::CS_GunTarget);
}

void UTargetAction::DoAction() {
	auto owner = GetOwner<AGamePlayerController>();
	auto pawn = owner->GetPawn();
	auto pawnCameraComponent = pawn->GetComponentByClass<UPawnCameraComponent>();

	if (!IsInValidCameraState(pawnCameraComponent->GetCurrentCameraState()))
		return;

	auto selected = SelectionManager->GetSelected();

	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates) 
		return UDebugMessages::LogError(this, "failed to get hud delegates, cannot do target action");

	// reset values
	if (pawnCameraComponent->GetCurrentCameraState() == ECameraState::CS_GunTarget) {
		pawnCameraComponent->UpdateCameraState(ECameraState::CS_Control);
		owner->SetMouseAsUi();
		hudDelegates->_AimTargetVisibility.Broadcast(false);
	}
	else if (selected) {
		auto actor = selected->GetAsActor();

		auto equipmentComponent = actor->GetComponentByClass<UEquipmentComponent>();
		auto detailsComponent = actor->GetComponentByClass<UCharacterDetailsComponent>();

		if (!equipmentComponent || !detailsComponent ||
			equipmentComponent->GetActionPointsNeededToUseEquipment() > detailsComponent->GetActionPoints())
			return UDebugMessages::LogError(this, "could not get equpiment or details component, or not enough action points. will not begin targeting");

		auto targetingComponent = actor->GetComponentByClass<UTargetingComponent>();
		if (targetingComponent) {
			auto currentTargetData = targetingComponent->GetCurrentMainTarget();
			if (!currentTargetData) {
				UDebugMessages::LogError(this, "no targeting data");
				return;
			}
			//assuming only one target data

			pawnCameraComponent->UpdateCameraState(ECameraState::CS_GunTarget, currentTargetData->GetShootLocation(),
				currentTargetData->GetCharacter()->GetSelectableGridLocation());

			owner->SetMouseAsGame();
			hudDelegates->_AimTargetVisibility.Broadcast(true);
		}
	}
}