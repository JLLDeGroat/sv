// Fill out your copyright notice in the Description page of Project Settings.


#include "SwapWeaponAction.h"
#include "../Managers/SelectionManager.h"
#include "../../Interfaces/SvChar.h"
#include "../../Interfaces/Selectable.h"
#include "../../Characters/Components/EquipmentComponent.h"

USwapWeaponAction::USwapWeaponAction(const FObjectInitializer& ObjectInitializer)
	: UBaseActionComponent(ObjectInitializer) {

	PrimaryComponentTick.bCanEverTick = false;
	ValidCameraStates.Emplace(ECameraState::CS_Control);
}

void USwapWeaponAction::DoAction() {
	if (IsWithinValidControlLimiter()) {
		ResetActionEffects();
		auto selected = SelectionManager->GetSelected();
		if (selected) {
			auto component = selected->GetAsActor()->GetComponentByClass<UEquipmentComponent>();
			component->SwapWeapon();
		}
	}
}