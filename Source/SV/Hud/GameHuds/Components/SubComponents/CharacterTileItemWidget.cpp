// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterTileItemWidget.h"
#include "../../../Helpers/EquipmentInventoryHelpers.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/Button.h"
#include "../../../../Player/Actions/Base/ActionManager.h"
#include "../../../../Player/Actions/LeftClickAction.h"
#include "../../../../Player/Managers/SelectionManager.h"
#include "../../../../Interfaces/Selectable.h"
#include "../../../../Characters/Components/ActionsComponent.h"
#include "../../../../Player/Components/PawnCameraComponent.h"
#include "Animation/WidgetAnimation.h"
#include "../../../../Delegates/HudDelegates.h"

void UCharacterTileItemWidget::NativeConstruct() {
	Super::NativeConstruct();

	auto button = UEquipmentInventoryHelpers::GetButtonFromWidget(this, "CharacterButton");

	if (!button)
		return UDebugMessages::LogError(this, "could not find character button");

	button->OnClicked.AddDynamic(this, &UCharacterTileItemWidget::OnCharacterButtonClicked);


	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "could not find hud delegates");

	hudDelegates->_ResetCharacterTileWidget.AddDynamic(this, &UCharacterTileItemWidget::Deactivate);
}

void UCharacterTileItemWidget::SetRepresentedActor(AActor* actor) {
	RepresentedActor = actor;
}

void UCharacterTileItemWidget::OnCharacterButtonClicked() {
	if (Activated)
		return;

	Activating = true;

	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "could not find hud delegates");
	hudDelegates->_ResetCharacterTileWidget.Broadcast();

	auto controller = GetWorld()->GetFirstPlayerController();
	auto manager = controller->GetComponentByClass<UActionManager>();

	auto selectionManager = controller->GetComponentByClass<USelectionManager>();

	if (!selectionManager)
		return UDebugMessages::LogError(this, "failed to get selection manager");

	if (RepresentedActor && selectionManager->TrySetSelected(RepresentedActor)) {
		selectionManager->GetSelected()->TryVisualiseTargets();

		auto actionsComponent = RepresentedActor->GetComponentByClass<UActionsComponent>();
		if (!actionsComponent)
			return UDebugMessages::LogError(this, "failed to get actions component");

		actionsComponent->SendActionsToUI();

		auto pawn = controller->GetPawn();
		if (pawn && pawn->GetComponentByClass<UPawnCameraComponent>())
			pawn->GetComponentByClass<UPawnCameraComponent>()->UpdateCameraState(ECameraState::CS_ReTarget, RepresentedActor->GetActorLocation());

		if (AnimateIn)
			PlayAnimationForward(AnimateIn);

		Activated = true;
		Activating = false;
	}
}

void UCharacterTileItemWidget::Deactivate() {
	if (AnimateOut && !Activating && Activated) {
		PlayAnimationForward(AnimateOut);
		Activated = false;
		UDebugMessages::LogDisplay(this, "Deactivate");
	}
}
