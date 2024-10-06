// Fill out your copyright notice in the Description page of Project Settings.


#include "SupplierListItemWidget.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "SupplierListCostItemWidget.h"
#include "../../../../../Helpers/UserWidgetHelpers.h"
#include "Components/TextBlock.h"
#include "Components/GridPanel.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "../../../../../../Instance/SvGameInstance.h"
#include "../../../../../../Instance/Managers/CurrentGameDataManager.h"
#include "../../../../../../Utilities/SvUtilities.h"
#include "../../../../../../Delegates/HudDelegates.h"

void USupplierListItemWidget::NativeConstruct() {
	Super::NativeConstruct();

	if (PurchaseButton) {
		UUserWidgetHelpers::DesignButton(PurchaseButton);
		PurchaseButton->OnClicked.AddDynamic(this, &USupplierListItemWidget::OnPurchaseClicked);
	}

	if (DescriptionText) UUserWidgetHelpers::DesignText(DescriptionText); 
	if (AmountOwnedText) UUserWidgetHelpers::DesignText(AmountOwnedText);
	if (ItemName) UUserWidgetHelpers::DesignText(ItemName);
}

void USupplierListItemWidget::SetNameOfItem(FString name) {
	if (!ItemName) return UDebugMessages::LogError(this, "could not find item name text block");

	ItemName->SetText(FText::FromString(name));
}
void USupplierListItemWidget::SetBigImage(UTexture2D* texture) {
	if (!Image)  return UDebugMessages::LogError(this, "could not find Image");

	Image->SetBrushFromTexture(texture);
}
void USupplierListItemWidget::SetDescriptionOfItem(FString desc) {
	if (!DescriptionText) return UDebugMessages::LogError(this, "could not find Description");

	DescriptionText->SetText(FText::FromString(desc));
}
void USupplierListItemWidget::SetAmountOwnedText(int amount) {
	AmountOwned = amount;

	if (!AmountOwnedText)
		return UDebugMessages::LogError(this, "could not find Amountowned text");

	AmountOwnedText->SetText(FText::FromString("owned: " + FString::SanitizeFloat(amount, 0)));
}

void USupplierListItemWidget::AddCosts(TMap<EResourceType, int> costs) {
	if (!CostGrid)
		return UDebugMessages::LogError(this, "failed to get cost grid panel");

	Costs = costs;

	for (const TPair<EResourceType, int>& pair : costs) {
		auto column = GetColumnBasedOnResourceType(pair.Key);
		auto newWidget = CreateCostItemWidget();
		if (newWidget) {
			auto resourceTexture = UUserWidgetHelpers::GetTextureForResourceType(pair.Key);
			auto stringValue = "x " + FString::SanitizeFloat(pair.Value, 0);

			if (!resourceTexture)
				UDebugMessages::LogWarning(this, "resource texture was nullptr");

			newWidget->SetResourceImage(resourceTexture);
			newWidget->SetResourceText(stringValue);

			CostGrid->AddChildToGrid(newWidget, 0, column);
		}
	}
}

USupplierListCostItemWidget* USupplierListItemWidget::CreateCostItemWidget() {
	FSoftClassPath hudUIRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/World/Components/Pages/Supplies/SubComponents/SupplyListCostItem_Bp.SupplyListCostItem_Bp_C'"));
	if (UClass* hudUIWidgetClass = hudUIRef.TryLoadClass<USupplierListCostItemWidget>())
		return CreateWidget<USupplierListCostItemWidget>(this, hudUIWidgetClass);

	return nullptr;
}

int USupplierListItemWidget::GetColumnBasedOnResourceType(EResourceType resource) {
	switch (resource) {
	case EResourceType::RT_Currency:
		return 0;
	default:
		UDebugMessages::LogError(this, "failed to get row on resource type");
		return 2;
	}
}

void USupplierListItemWidget::ShortenImage() {
	if (!Image)
		return UDebugMessages::LogError(this, "could not find Image");

	auto currentSize = Image->GetBrush().ImageSize;
	Image->SetBrushSize(FVector2D(300, currentSize.Y));
}

void USupplierListItemWidget::OnPurchaseClicked() {
	UDebugMessages::LogDisplay(this, "Purchased");

	auto currentGameData = USvUtilities::GetCurrentGameData(GetWorld());
	auto resources = currentGameData->GetResourceData();

	bool bCanAfford = GetCanAfford();

	if (bCanAfford) {
		for (const TPair<EResourceType, int>& pair : Costs) {
			auto resItem = resources->GetResource(pair.Key);
			resItem->RemoveFromAmount(pair.Value);

			auto hudDelegates = UHudDelegates::GetInstance();
			if (!hudDelegates)
				return UDebugMessages::LogError(this, "failed to get hud delegates");

			hudDelegates->_OnResourceChange.Broadcast(pair.Key);
		}
	}
	else {
		UDebugMessages::LogError(this, "can not afford");
		return;
	}

	UDebugMessages::LogDisplay(this, FString::SanitizeFloat((uint8)StockItem.GetGunType()));

	if (StockItem.GetGunType() != EGun::INVALID) {
		currentGameData->AddPrimaryToCrew(StockItem.GetGunType());
	}
	if (StockItem.GetThrowableType() != EThrowable::INVALID) {
		currentGameData->AddToolToCrew(EToolType::TT_Throwable, (uint8)StockItem.GetThrowableType());
	}
	if (StockItem.GetToolType() != EToolType::INVALID) {
		UDebugMessages::LogError(this, "TODO Other tools or usables");
	}

	AmountOwned += 1;
	SetAmountOwnedText(AmountOwned);
}

void USupplierListItemWidget::SetStockItem(FSupplyStockItem* stockItem) {
	StockItem = *stockItem;
}

bool USupplierListItemWidget::GetCanAfford() {
	bool bCanAfford = true;
	auto currentGameData = USvUtilities::GetCurrentGameData(GetWorld());
	auto resources = currentGameData->GetResourceData();

	for (const TPair<EResourceType, int>& pair : Costs) {
		auto resItem = resources->GetResource(pair.Key);
		if (resItem->GetAmount() < pair.Value) {
			bCanAfford = false;
		}
	}

	return bCanAfford;
}