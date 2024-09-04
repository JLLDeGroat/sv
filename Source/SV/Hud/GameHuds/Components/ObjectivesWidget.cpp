// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectivesWidget.h"
#include "../../Helpers/UserWidgetHelpers.h"
#include "../../../Delegates/HudDelegates.h"
#include "Components/TextBlock.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

void UObjectivesWidget::NativeConstruct() {
	Super::NativeConstruct();
	/*UpdateMainObjective("");
	UpdateSecondaryObjective("");
	UpdateTertiaryObjective("");*/

	auto hudDelegates = UHudDelegates::GetInstance();
	if (hudDelegates) {
		hudDelegates->_UpdateMainObjectiveText.AddDynamic(this, &UObjectivesWidget::UpdateMainObjective);
		hudDelegates->_UpdateSubObjectiveOneText.AddDynamic(this, &UObjectivesWidget::UpdateSecondaryObjective);
		hudDelegates->_UpdateSubObjectiveTwoText.AddDynamic(this, &UObjectivesWidget::UpdateTertiaryObjective);
	}
}

void UObjectivesWidget::UpdateMainObjective(FString str) {
	UpdateTextItem(str, "MainObjectiveText");
}
void UObjectivesWidget::UpdateSecondaryObjective(FString str) {
	UpdateTextItem(str, "SubObjectiveTextOne");
}
void UObjectivesWidget::UpdateTertiaryObjective(FString str) {
	UpdateTextItem(str, "SubObjectiveTextTwo");
}

void UObjectivesWidget::UpdateTextItem(FString str, FString textName) {
	UDebugMessages::LogDisplay(this, "updating text for " + textName);
	auto textBlock = UUserWidgetHelpers::GetTextBlockFromWidget(this, textName);
	if (textBlock) {
		textBlock->SetText(FText::FromString(str));
		textBlock->SetVisibility(str.Len() > 0 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UObjectivesWidget::BeginDestroy() {
	auto hudDelegates = UHudDelegates::GetInstance();
	if (hudDelegates) {
		hudDelegates->_UpdateMainObjectiveText.RemoveDynamic(this, &UObjectivesWidget::UpdateMainObjective);
		hudDelegates->_UpdateSubObjectiveOneText.RemoveDynamic(this, &UObjectivesWidget::UpdateSecondaryObjective);
		hudDelegates->_UpdateSubObjectiveTwoText.RemoveDynamic(this, &UObjectivesWidget::UpdateTertiaryObjective);
	}
	Super::BeginDestroy();
}