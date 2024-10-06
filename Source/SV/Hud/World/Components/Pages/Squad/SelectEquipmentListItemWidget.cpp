// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectEquipmentListItemWidget.h"
#include "../../../../Helpers/UserWidgetHelpers.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "SelectEquipmentListWidget.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

void USelectEquipmentListItemWidget::NativeConstruct() {
	Super::NativeConstruct();

	if (AmountText)UUserWidgetHelpers::DesignText(AmountText);
}

void USelectEquipmentListItemWidget::AddPrimaryDelegate() {
	auto button = GetActivateButton();
	button->OnClicked.AddDynamic(this, &USelectEquipmentListItemWidget::ButtonClickedPrimary);

	UDebugMessages::LogDisplay(this, "added primary delegate");
}
void USelectEquipmentListItemWidget::AddToolDelegate() {
	auto button = GetActivateButton();
	button->OnClicked.AddDynamic(this, &USelectEquipmentListItemWidget::ButtonClickedTool);

	UDebugMessages::LogDisplay(this, "added tool delegate");
}

EGun USelectEquipmentListItemWidget::GetPrimaryGun() {
	return PrimaryGun;
}
void USelectEquipmentListItemWidget::SetPrimaryGun(EGun primary) {
	PrimaryGun = primary;

	auto image = UUserWidgetHelpers::GetImageFromWidget(this, "SelectionImage");
	if (image) {
		auto texture = UUserWidgetHelpers::GetTextureForGun(primary);
		image->SetBrushFromTexture(texture);
	}
}

FGuid USelectEquipmentListItemWidget::GetItemId() {
	return ItemId;
}
void USelectEquipmentListItemWidget::SetItemId(FGuid id) {
	ItemId = id;
}

void USelectEquipmentListItemWidget::SetToolAndType(EToolType toolType, uint8 tool) {
	ToolType = toolType;
	Tool = tool;
	auto image = UUserWidgetHelpers::GetImageFromWidget(this, "SelectionImage");
	if (image) {
		auto texture = UUserWidgetHelpers::GetTextureForTool(toolType, tool);
		image->SetBrushFromTexture(texture);
	}
}
EToolType USelectEquipmentListItemWidget::GetToolType() {
	return ToolType;
}

uint8 USelectEquipmentListItemWidget::GetTool() {
	return Tool;
}

int USelectEquipmentListItemWidget::AddToAmount(int amount) {
	Amount += amount;

	if (AmountText)
		AmountText->SetText(FText::FromString("x " + FString::SanitizeFloat(Amount, 0)));

	return Amount;
}

void USelectEquipmentListItemWidget::ButtonClickedPrimary() {
	auto  owner = (USelectEquipmentListWidget*)GetOuter()->GetOuter();
	if (owner) {
		UDebugMessages::LogDisplay(this, "primary button clicked");
		owner->PrimaryButtonClicked(PrimaryGun, ItemId);
	}
	else return UDebugMessages::LogError(this, "failed to find owner");
}
void USelectEquipmentListItemWidget::ButtonClickedTool() {
	auto  owner = (USelectEquipmentListWidget*)GetOuter()->GetOuter();
	if (owner) {
		UDebugMessages::LogDisplay(this, "primary button clicked");
		owner->ToolButtonClicked(ToolType, Tool);
	}
	else return UDebugMessages::LogError(this, "failed to find owner");
}

UButton* USelectEquipmentListItemWidget::GetActivateButton() {
	return UUserWidgetHelpers::GetButtonFromWidget(this, "ActivateButton");
}