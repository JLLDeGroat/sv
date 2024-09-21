// Fill out your copyright notice in the Description page of Project Settings.


#include "SupplierPageWidget.h"
#include "Components/Pages/Supplier/SupplyListWidget.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

void USupplierPageWidget::NativeConstruct() {
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Hidden);
}
void USupplierPageWidget::OpenPage() {
	SetVisibility(ESlateVisibility::Visible);
	if (!SupplyList)
		return UDebugMessages::LogError(this, "supplier list in not valid");

	SupplyList->FillSupplyList();
}