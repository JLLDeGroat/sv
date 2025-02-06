// Fill out your copyright notice in the Description page of Project Settings.


#include "CycleTargetAction.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Managers/SelectionManager.h"
#include "../../Delegates/HudDelegates.h"
#include "../Components/PawnCameraComponent.h"

UCycleTargetAction::UCycleTargetAction(const FObjectInitializer& ObjectInitializer)
	:UBaseActionComponent(ObjectInitializer) {

	ValidCameraStates.Emplace(ECameraState::CS_Control);
	ValidCameraStates.Emplace(ECameraState::CS_GunTarget);
}

void UCycleTargetAction::DoAction() {
	if (IsWithinValidControlLimiter() && IsInValidCameraState()) {
		ResetActionEffects();

		auto pawn = GetOwningController()->GetPawn();
		auto pawnCamera = pawn->GetComponentByClass<UPawnCameraComponent>();
		auto currentCameraState = pawnCamera->GetCurrentCameraState();

		//auto selected = SelectionManager->GetSelected();

		auto hudDelegates = UHudDelegates::GetInstance();
		if (!hudDelegates)
			return UDebugMessages::LogError(this, "failed to get hud delegates");

		if (currentCameraState == ECameraState::CS_Control)
			hudDelegates->_SelectNextCharacterWithAp.Broadcast();
		else
			hudDelegates->_CycleToNextTarget.Broadcast();
		
		/*if (selected)
			hudDelegates->_CycleToNextTarget.Broadcast();
		else
			hudDelegates->_SelectNextCharacterWithAp.Broadcast();*/
	}
}