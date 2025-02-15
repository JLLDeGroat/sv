// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionListWidget.h"
#include "../../../Delegates/HudDelegates.h"
#include "SubComponents/ActionListItemWidget.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/VerticalBox.h"
#include "Components/GridPanel.h"
#include "Components/TextBlock.h"
#include "../../Helpers/UserWidgetHelpers.h"

void UActionListWidget::NativeConstruct() {
	Super::NativeConstruct();

	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "failed to get hud delegates");

	hudDelegates->_AddActionIconToHud.AddDynamic(this, &UActionListWidget::CreateAndAddActionItem);
	hudDelegates->_ResetActionIcons.AddDynamic(this, &UActionListWidget::ClearVerticalBox);

	hudDelegates->_HideOrResetUIWidget.AddDynamic(this, &UActionListWidget::HideOrReset);

	DescriptionGrid->SetVisibility(ESlateVisibility::Hidden);

	UUserWidgetHelpers::DesignText(DescriptionText, 19);
	UUserWidgetHelpers::DesignText(NameText);
}

void UActionListWidget::CreateAndAddActionItem(EActionType actionType, FString shortCut) {
	auto newListItem = CreateListItem();
	auto verticalBox = (UVerticalBox*)GetWidgetFromName(FName("ActionsList"));
	if (!newListItem || !verticalBox)
		return UDebugMessages::LogError(this, "failed to get action vertical box or create new list item");

	newListItem->SetupActionItem(actionType, shortCut);
	verticalBox->AddChildToVerticalBox(newListItem);
}
void UActionListWidget::HideOrReset() {
	ClearVerticalBox();
}

UActionListItemWidget* UActionListWidget::CreateListItem() {
	FSoftClassPath hudUIRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/Components/SubComponents/ActionListItemWidget_Bp.ActionListItemWidget_Bp_C'"));
	if (UClass* hudUIWidgetClass = hudUIRef.TryLoadClass<UActionListItemWidget>())
		return CreateWidget<UActionListItemWidget>(this, hudUIWidgetClass);

	return nullptr;
}

void UActionListWidget::ClearVerticalBox() {
	auto verticalBox = (UVerticalBox*)GetWidgetFromName(FName("ActionsList"));

	if (!verticalBox)
		return UDebugMessages::LogError(this, "failed to get vertical box");

	DescriptionGrid->SetVisibility(ESlateVisibility::Hidden);
	verticalBox->ClearChildren();
}

void UActionListWidget::ShowDescriptionWidgetWithData(FString name, FString desc) {
	NameText->SetText(FText::FromString(name));
	DescriptionText->SetText(FText::FromString(desc));

	DescriptionGrid->SetVisibility(ESlateVisibility::Visible);
}

void UActionListWidget::HideDescriptionWidget() {
	DescriptionGrid->SetVisibility(ESlateVisibility::Hidden);
}