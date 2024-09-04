// Fill out your copyright notice in the Description page of Project Settings.


#include "SleepAction.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Managers/SelectionManager.h"
#include "../../Interfaces/Selectable.h"
#include "../../Delegates/HudDelegates.h"

#include "../../Characters/Components/CharacterDetailsComponent.h"

USleepAction::USleepAction(const FObjectInitializer& ObjectInitializer)
	: UBaseActionComponent(ObjectInitializer) {

}

void USleepAction::DoAction() {
	if (IsWithinValidControlLimiter()) {
		ResetActionEffects();


		auto selected = SelectionManager->GetSelected();

		auto hudDelegates = UHudDelegates::GetInstance();
		if (!hudDelegates)
			return UDebugMessages::LogError(this, "failed to get hud delegates");

		if (selected) {
			auto details = selected->GetAsActor()->GetComponentByClass<UCharacterDetailsComponent>();
			if (details) {
				details->RemoveMovementPoints(details->GetMaxMovementPoints());
				details->RemoveActionPoints(details->GetMaxActionPoints());

				SelectionManager->TrySetSelected(nullptr);

				hudDelegates->_HideOrResetUIWidget.Broadcast();
				hudDelegates->_ResetCharacterTileWidget.Broadcast();
				hudDelegates->_OnHudItemUnhovered.Broadcast();
			}
		}
	}
}