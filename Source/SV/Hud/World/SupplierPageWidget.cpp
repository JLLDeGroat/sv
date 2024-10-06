// Fill out your copyright notice in the Description page of Project Settings.


#include "SupplierPageWidget.h"
#include "Components/Pages/Supplier/SupplyListWidget.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Delegates/TutorialDelegates.h"

void USupplierPageWidget::NativeConstruct() {
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Hidden);
}
void USupplierPageWidget::OpenPage() {
	SetVisibility(ESlateVisibility::Visible);
	if (!SupplyList)
		return UDebugMessages::LogError(this, "supplier list in not valid");

	auto tutorialDelegates = UTutorialDelegates::GetInstance();
	if (!tutorialDelegates)
		return UDebugMessages::LogError(this, "failed to get tutorial delegates");

	tutorialDelegates->_OnTryShowTutorial.Broadcast(ETutorials::T_Supplies);

	SupplyList->FillSupplyList();
}