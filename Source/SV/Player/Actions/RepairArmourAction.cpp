// Fill out your copyright notice in the Description page of Project Settings.

#include "RepairArmourAction.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Components/PawnCameraComponent.h"
#include "../../Interfaces/Selectable.h"
#include "../Managers/SelectionManager.h"

#include "../../Characters/Components/EquipmentComponent.h"
#include "../../Characters/Components/CharacterDetailsComponent.h"
#include "../../Equipment/Components/EquipmentDetailsComponent.h"
#include "../../Equipment/Equipment.h"

URepairArmourAction::URepairArmourAction(const FObjectInitializer &ObjectInitializer)
	: UBaseActionComponent(ObjectInitializer)
{
	ValidCameraStates.Emplace(ECameraState::CS_Control);
}

void URepairArmourAction::DoAction()
{
	if (IsWithinValidControlLimiter())
	{
		UDebugMessages::LogDisplay(this, "doing repair action");
		ResetActionEffects();
	}
}