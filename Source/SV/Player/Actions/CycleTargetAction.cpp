// Fill out your copyright notice in the Description page of Project Settings.


#include "CycleTargetAction.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Managers/SelectionManager.h"
#include "../../Delegates/HudDelegates.h"


UCycleTargetAction::UCycleTargetAction(const FObjectInitializer& ObjectInitializer)
	:UBaseActionComponent(ObjectInitializer) {

}

void UCycleTargetAction::DoAction() {
	auto selected = SelectionManager->GetSelected();
	if (selected) {
		auto hudDelegates = UHudDelegates::GetInstance();
		if (!hudDelegates)
			return UDebugMessages::LogError(this, "failed to get hud delegates");

		hudDelegates->_CycleToNextTarget.Broadcast();
	}
}