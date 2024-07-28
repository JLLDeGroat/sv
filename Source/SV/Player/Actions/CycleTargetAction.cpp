// Fill out your copyright notice in the Description page of Project Settings.


#include "CycleTargetAction.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Managers/SelectionManager.h"
#include "../../Delegates/HudDelegates.h"


UCycleTargetAction::UCycleTargetAction(const FObjectInitializer& ObjectInitializer)
	:UBaseActionComponent(ObjectInitializer) {

}

void UCycleTargetAction::DoAction() {
	ResetActionEffects();

	auto selected = SelectionManager->GetSelected();

	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "failed to get hud delegates");

	if (selected)
		hudDelegates->_CycleToNextTarget.Broadcast();
	else
		hudDelegates->_SelectNextCharacterWithAp.Broadcast();
}