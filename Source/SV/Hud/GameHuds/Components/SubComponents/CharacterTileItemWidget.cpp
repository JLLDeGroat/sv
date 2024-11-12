// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterTileItemWidget.h"
#include "../../../Helpers/UserWidgetHelpers.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/Button.h"
#include "../../../../Player/Actions/Base/ActionManager.h"
#include "../../../../Player/Actions/LeftClickAction.h"
#include "../../../../Player/Managers/SelectionManager.h"
#include "../../../../Interfaces/Selectable.h"
#include "../../../../Characters/Components/ActionsComponent.h"
#include "../../../../Characters/Components/CharacterDetailsComponent.h"
#include "../../../../Player/Components/PawnCameraComponent.h"
#include "Animation/WidgetAnimation.h"
#include "../../../../Delegates/HudDelegates.h"
#include "../../../../Characters/Components/CharacterCaptureComponent.h"
#include "Components/Image.h"
#include "Kismet/KismetRenderingLibrary.h"

void UCharacterTileItemWidget::NativeConstruct() {
	Super::NativeConstruct();

	if (!CharacterButton)
		return UDebugMessages::LogError(this, "could not find character button");

	CharacterButton->OnClicked.AddDynamic(this, &UCharacterTileItemWidget::OnCharacterButtonClicked);
	CharacterButton->OnHovered.AddDynamic(this, &UCharacterTileItemWidget::OnHovered);
	CharacterButton->OnUnhovered.AddDynamic(this, &UCharacterTileItemWidget::OnUnhovered);

	DeathImage->SetVisibility(ESlateVisibility::Hidden);

	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "could not find hud delegates");

	hudDelegates->_ResetCharacterTileWidget.AddDynamic(this, &UCharacterTileItemWidget::Deactivate);
	hudDelegates->_CheckCharacterTileIsActive.AddDynamic(this, &UCharacterTileItemWidget::SimulateOnCharacterButtonClicked);

	hudDelegates->_OnSoldierDeath.AddDynamic(this, &UCharacterTileItemWidget::OnSodlierDeath);
}

void UCharacterTileItemWidget::SetRepresentedActor(AActor* actor) {
	RepresentedActor = actor;
	UpdateCameraCallback();
}

void UCharacterTileItemWidget::UpdateCameraCallback() {
	auto captureComponent = RepresentedActor->GetComponentByClass<UCharacterCaptureComponent>();
	if (captureComponent && CharacterImage) {
		CurrentCameraTexture = captureComponent->ConvertRenderTargetToTexture2D();
		CharacterImage->SetBrushFromTexture(CurrentCameraTexture);
	}
}

AActor* UCharacterTileItemWidget::GetRepresentedActor() {
	return RepresentedActor;
}
void UCharacterTileItemWidget::SimulateOnCharacterButtonClicked(AActor* actor) {
	if (actor == RepresentedActor)
		OnCharacterButtonClicked();
}

void UCharacterTileItemWidget::OnCharacterButtonClicked() {
	if (Activated)
		return;

	if (RepresentedActor) {
		auto details = RepresentedActor->GetComponentByClass<UCharacterDetailsComponent>();
		if (details && !details->GetIsDead()) {

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
	}
}

void UCharacterTileItemWidget::OnHovered() {
	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "could not get hud delegeats, cannot do target icon clicked");

	hudDelegates->_OnHudItemHovered.Broadcast();
}
void UCharacterTileItemWidget::OnUnhovered() {
	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "could not get hud delegeats, cannot do target icon clicked");

	hudDelegates->_OnHudItemUnhovered.Broadcast();
}

void UCharacterTileItemWidget::Deactivate() {
	if (AnimateOut && !Activating && Activated) {
		PlayAnimationForward(AnimateOut);
		Activated = false;
		UDebugMessages::LogDisplay(this, "Deactivate");
	}
}

void UCharacterTileItemWidget::OnSodlierDeath(AActor* DeadSoldier) {
	if (DeadSoldier == RepresentedActor)
		DeathImage->SetVisibility(ESlateVisibility::Visible);
}