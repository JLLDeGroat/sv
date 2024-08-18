// Fill out your copyright notice in the Description page of Project Settings.


#include "EndTurnWidget.h"
#include "Components/Button.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Helpers/UserWidgetHelpers.h"
#include "../../Delegates/HudDelegates.h"
#include "../../GameModes/GameplayMode.h"
#include "../../Interfaces/Gameplay.h"

void UEndTurnWidget::NativeConstruct() {
	Super::NativeConstruct();

	auto btn = GetWidgetFromName("EndTurnBtn");

	if (btn && btn->IsA<UButton>()) {
		auto thisButton = (UButton*)btn;
		thisButton->OnClicked.AddDynamic(this, &UEndTurnWidget::EndTurnClicked);
		thisButton->OnHovered.AddDynamic(this, &UEndTurnWidget::OnButtonHovered);
		thisButton->OnUnhovered.AddDynamic(this, &UEndTurnWidget::OnButtonUnHovered);
	}
	else UDebugMessages::LogDisplay(this, "failed to cast to button");

	UUserWidgetHelpers::DesignButton(this, "EndTurnBtn");
}

void UEndTurnWidget::EndTurnClicked() {
	//TODO either do pre checks here or at the end
	TScriptInterface<IGameplay> gamePlay = GetWorld()->GetAuthGameMode();
	if (gamePlay) {
		gamePlay->EndTurn();
	}
}

void UEndTurnWidget::OnButtonUnHovered() {
	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "could not get hud delegeats, cannot do target icon clicked");

	hudDelegates->_OnHudItemUnhovered.Broadcast();
}
void UEndTurnWidget::OnButtonHovered() {
	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "could not get hud delegeats, cannot do target icon clicked");

	hudDelegates->_OnHudItemHovered.Broadcast();
}
