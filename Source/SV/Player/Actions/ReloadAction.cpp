// Fill out your copyright notice in the Description page of Project Settings.


#include "ReloadAction.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Components/PawnCameraComponent.h"
#include "../../Interfaces/Selectable.h"
#include "../Managers/SelectionManager.h"

#include "../../Characters/Components/EquipmentComponent.h"
#include "../../Characters/Components/CharacterDetailsComponent.h"
#include "../../Equipment/Components/EquipmentDetailsComponent.h"
#include "../../Equipment/Equipment.h"

UReloadAction::UReloadAction(const FObjectInitializer& ObjectInitializer)
	: UBaseActionComponent(ObjectInitializer) {

	ValidCameraStates.Emplace(ECameraState::CS_Control);
}

void UReloadAction::DoAction() {
	UDebugMessages::LogDisplay(this, "doing reload");
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

		auto characterDetails = selectedAsActor->GetComponentByClass<UCharacterDetailsComponent>();
		if (!characterDetails)
			return UDebugMessages::LogError(this, "failed to get character details");

		auto equipmentComponent = selectedAsActor->GetComponentByClass<UEquipmentComponent>();
		auto equipment = equipmentComponent->GetPrimaryEquipment();

		if (!equipment)
			return UDebugMessages::LogError(this, "failed to get equipment");

		auto equipmentDetails = equipment->GetComponentByClass<UEquipmentDetailsComponent>();
		if (!equipmentDetails)
			return UDebugMessages::LogError(this, "failed to get equipmentDetails");


		if (characterDetails->GetActionPoints() >= equipmentDetails->GetReloadApCost()) {
			equipmentComponent->ReloadEquipment();
		}
		else
			return UDebugMessages::LogError(this, "cannot reload, not enough ap");
	}
}