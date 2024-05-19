// Fill out your copyright notice in the Description page of Project Settings.


#include "EndTurnWidget.h"
#include "Components/Button.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../GameModes/GameplayMode.h"
#include "../../Interfaces/Gameplay.h"

void UEndTurnWidget::NativeConstruct() {
	Super::NativeConstruct();

	auto btn = GetWidgetFromName("EndTurnBtn");

	if (btn && btn->IsA<UButton>()) {
		auto thisButton = (UButton*)btn;
		thisButton->OnClicked.AddDynamic(this, &UEndTurnWidget::EndTurnClicked);
	}
	else UDebugMessages::LogDisplay(this, "failed to cast to button");
}

void UEndTurnWidget::EndTurnClicked() {
	//TODO either do pre checks here or at the end

	TScriptInterface<IGameplay> gamePlay = GetWorld()->GetAuthGameMode();
	if (gamePlay) {
		gamePlay->EndTurn();
	}
}