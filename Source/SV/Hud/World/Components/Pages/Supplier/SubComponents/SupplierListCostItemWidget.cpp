// Fill out your copyright notice in the Description page of Project Settings.


#include "SupplierListCostItemWidget.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../../../Helpers/UserWidgetHelpers.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USupplierListCostItemWidget::NativeConstruct() {
	Super::NativeConstruct();

	if (ResourceText) UUserWidgetHelpers::DesignText(ResourceText);
}

void USupplierListCostItemWidget::SetResourceImage(UTexture2D* texture) {
	if (!ResourceImage) return UDebugMessages::LogError(this, "failed to get resource image");
	ResourceImage->SetBrushFromTexture(texture);
}
void USupplierListCostItemWidget::SetResourceText(FString text) {
	if (!ResourceText) return UDebugMessages::LogError(this, "failed to get resource image");
	ResourceText->SetText(FText::FromString(text));
}