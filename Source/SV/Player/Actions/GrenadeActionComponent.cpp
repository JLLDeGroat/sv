// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeActionComponent.h"
#include "../Managers/SelectionManager.h"
#include "../Managers/ControlManager.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

#include "../../Characters/Components/ThrowableComponent.h"
#include "../../Characters/Components/CharacterDetailsComponent.h"
#include "../../Interfaces/Selectable.h"
#include "../Utility/GrenadeIndicatorActor.h"
#include "../Components/PawnCameraComponent.h"
#include "Camera/CameraComponent.h"

UGrenadeActionComponent::UGrenadeActionComponent(const FObjectInitializer& ObjectInitializer) :UBaseActionComponent(ObjectInitializer) {
	ValidCameraStates.Emplace(ECameraState::CS_Control);
	ValidCameraStates.Emplace(ECameraState::CS_ThrowTarget);
}


void UGrenadeActionComponent::SetGrenadeIndicatorActor(AGrenadeIndicatorActor* actor) {
	GrenadeIndicatorActor = actor;
}

void UGrenadeActionComponent::DoAction() {
	ResetActionEffects();

	auto pawn = GetOwningController()->GetPawn();
	auto pawnCamera = pawn->GetComponentByClass<UPawnCameraComponent>();

	if (!IsInValidCameraState(pawnCamera->GetCurrentCameraState()))
		return;

	if (pawnCamera->GetCurrentCameraState() == ECameraState::CS_ThrowTarget) {
		pawnCamera->UpdateCameraState(ECameraState::CS_Control,
			FVector::ZeroVector, FVector::ZeroVector, true);

		ControlManager->SetCanMouseDesignateSelectionDecal(true);
		ControlManager->SetCanMouseDesignateExplosionRadiusActor(false);
	}
	else {
		auto selected = SelectionManager->GetSelected();

		if (!selected)
			return UDebugMessages::LogError(this, "no selected user, cannot throw grenade");

		auto actor = selected->GetAsActor();
		auto throwableComponent = selected->GetAsActor()->GetComponentByClass<UThrowableComponent>();
		ThrowableChosen = EThrowable::T_Grenade;
		auto throwable = throwableComponent->GetThrowableItem(ThrowableChosen);

		if (!throwable)
			return UDebugMessages::LogError(this, "could not find throwable");

		auto detailsComponent = actor->GetComponentByClass<UCharacterDetailsComponent>();

		if (throwable->GetApCost() > detailsComponent->GetActionPoints())
			return UDebugMessages::LogError(this, "cannot throw grenade, not enough actions points");

		pawnCamera->UpdateCameraState(ECameraState::CS_ThrowTarget,
			FVector::ZeroVector, FVector::ZeroVector, true);

		ControlManager->SetCanMouseDesignateSelectionDecal(false);

		if (!selected)
			return UDebugMessages::LogError(this, "wont begin grenade, nothing selected");

		if (throwableComponent && throwableComponent->GetThrowableAmount(ThrowableChosen) > 0) {
			ControlManager->SetCanMouseDesignateExplosionRadiusActor(true);

			if (!throwable)
				return UDebugMessages::LogError(this, "failed to get throwable from component");

			GrenadeIndicatorActor->SetRange(throwable->GetRange());
			GrenadeIndicatorActor->SetActorScale3D(FVector(throwable->GetRadius()));
		}
		else
			return UDebugMessages::LogError(this, "had no throwable component of had zero grenades");
	}
}

EThrowable UGrenadeActionComponent::GetThrowableChosen() {
	return ThrowableChosen;
}