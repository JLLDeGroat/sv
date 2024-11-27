// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionListItemWidget.h"
#include "../../../Helpers/UserWidgetHelpers.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

#include "../../../../Player/GamePlayerController.h"
#include "../../../../Player/Actions/Base/ActionManager.h"
#include "../../../../Delegates/HudDelegates.h"
#include "../ActionListWidget.h"

void UActionListItemWidget::NativeConstruct() {
	Super::NativeConstruct();


}

void UActionListItemWidget::SetupActionItem(EActionType actionType, FString shortCut) {
	Action = actionType;
	ShortCut = shortCut;


	auto button = UUserWidgetHelpers::GetButtonFromWidget(this, "ActionButton");
	if (button) {
		button->OnClicked.AddDynamic(this, &UActionListItemWidget::OnButtonClick);
		button->OnHovered.AddDynamic(this, &UActionListItemWidget::OnButtonHovered);
		button->OnUnhovered.AddDynamic(this, &UActionListItemWidget::OnButtonUnHovered);
	}
	auto shortCutText = UUserWidgetHelpers::GetTextBlockFromWidget(this, "ActionShortCut");
	if (shortCutText)
		shortCutText->SetText(FText::FromString(shortCut));

	auto textureForAction = UUserWidgetHelpers::GetTextureForActionType(actionType);
	if (textureForAction) {
		auto imageWidget = UUserWidgetHelpers::GetImageFromWidget(this, "Image");
		if (imageWidget)
			imageWidget->SetBrushFromTexture(textureForAction);
	}
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

void UActionListItemWidget::OnButtonUnHovered() {
	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "could not get hud delegeats, cannot do target icon clicked");

	hudDelegates->_OnHudItemUnhovered.Broadcast();
	auto parent = (UActionListWidget*)GetOuter()->GetOuter();
	if (parent) {
		parent->HideDescriptionWidget();
	}
}
#pragma optimize("", off)
void UActionListItemWidget::OnButtonHovered() {
	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "could not get hud delegeats, cannot do target icon clicked");

	auto parent = (UActionListWidget*)GetOuter()->GetOuter();
	if (parent) {
		FString name, desc;
		UUserWidgetHelpers::GetNameAndDescriptionFromAction(Action, name, desc);

		parent->ShowDescriptionWidgetWithData(name, desc);
	}


	hudDelegates->_OnHudItemHovered.Broadcast();
}
#pragma optimize("", on)