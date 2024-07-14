// Fill out your copyright notice in the Description page of Project Settings.


#include "ActivateToggleAction.h"
#include "../Managers/SelectionManager.h"
#include "../../Characters/Components/ActivateTogglesComponent.h"
#include "../../Environment/Components/ToggleMovementComponent.h"
#include "../../Interfaces/Selectable.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

UActivateToggleAction::UActivateToggleAction(const FObjectInitializer& ObjectInitializer)
	: UBaseActionComponent(ObjectInitializer) {

}

void UActivateToggleAction::DoAction() {
	ResetActionEffects();

	auto selected = SelectionManager->GetSelected();

	if (selected) {
		auto actor = selected->GetAsActor();
		auto comp = actor->GetComponentByClass<UActivateTogglesComponent>();
		if (comp) {
			auto allToggles = comp->GetToggleComponents();

			//TODO do specific toggle
			for (int i = 0; i < allToggles.Num(); i++)
				if (allToggles[i])
					allToggles[i]->ActivateToggle();
		}
	}
}