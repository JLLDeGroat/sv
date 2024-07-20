// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionListItemWidget.h"
#include "../../../Helpers/EquipmentInventoryHelpers.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

#include "../../../../Player/GamePlayerController.h"
#include "../../../../Player/Actions/Base/ActionManager.h"

void UActionListItemWidget::NativeConstruct() {
	Super::NativeConstruct();


}

void UActionListItemWidget::SetupActionItem(EActionType actionType, FString shortCut) {
	Action = actionType;
	ShortCut = shortCut;


	auto button = UEquipmentInventoryHelpers::GetButtonFromWidget(this, "ActionButton");
	if (button)
		button->OnClicked.AddDynamic(this, &UActionListItemWidget::OnButtonClick);

	auto shortCutText = UEquipmentInventoryHelpers::GetTextBlockFromWidget(this, "ActionShortCut");
	if (shortCutText)
		shortCutText->SetText(FText::FromString(shortCut));
}

void UActionListItemWidget::OnButtonClick() {
	UDebugMessages::LogDisplay(this, "OnButtonClicked");

	auto owner = GetOwningPlayer<AGamePlayerController>();
	if (!owner)
		return UDebugMessages::LogError(this, "failed to get owning controller");

	auto actionManager = owner->GetComponentByClass<UActionManager>();
	if (!actionManager)
		return UDebugMessages::LogError(this, "failed to get action manager");

	actionManager->DoActionFromUI(Action);
}