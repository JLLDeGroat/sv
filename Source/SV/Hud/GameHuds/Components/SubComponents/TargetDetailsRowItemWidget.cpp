// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetDetailsRowItemWidget.h"
#include "../../../../Delegates/HudDelegates.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../../Player/GamePlayerController.h"
#include "../../../../Player/Managers/ControlManager.h"

#include "Components/Button.h"

void UTargetDetailsRowItemWidget::NativeConstruct() {
	Super::NativeConstruct();

	auto btn = GetItemButton();
	if (btn) {
		btn->OnClicked.AddDynamic(this, &UTargetDetailsRowItemWidget::OnButtonClicked);
		btn->OnHovered.AddDynamic(this, &UTargetDetailsRowItemWidget::OnButtonHovered);
		btn->OnUnhovered.AddDynamic(this, &UTargetDetailsRowItemWidget::OnButtonUnHovered);
	}
}

void UTargetDetailsRowItemWidget::SetSourceLocation(FVector sourceLocation) {
	SourceLocation = sourceLocation;
}
void UTargetDetailsRowItemWidget::SetTargetLocation(FVector targetLocation) {
	TargetLocation = targetLocation;
}
void UTargetDetailsRowItemWidget::SetId(FGuid id) {
	Id = id;
}

FVector UTargetDetailsRowItemWidget::GetSourceLocation() const {
	return SourceLocation;
}
FVector UTargetDetailsRowItemWidget::GetTargetLocation() const {
	return TargetLocation;
}
FGuid UTargetDetailsRowItemWidget::GetId() const {
	return Id;
}

void UTargetDetailsRowItemWidget::OnButtonClicked() {
	auto hudDelegates = UHudDelegates::GetInstance();

	if (!hudDelegates)
		return UDebugMessages::LogError(this, "could not get hud delegeats, cannot do target icon clicked");

	hudDelegates->_TargetIconClicked.Broadcast(Id, TargetLocation);
}

void UTargetDetailsRowItemWidget::OnButtonUnHovered() {
	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "could not get hud delegeats, cannot do target icon clicked");

	hudDelegates->_OnHudItemUnhovered.Broadcast();
}
void UTargetDetailsRowItemWidget::OnButtonHovered() {
	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "could not get hud delegeats, cannot do target icon clicked");

	hudDelegates->_OnHudItemHovered.Broadcast();
}


UButton* UTargetDetailsRowItemWidget::GetItemButton() const {
	auto btn = GetWidgetFromName("Button");
	if (btn->IsA<UButton>())
		return (UButton*)btn;
	return nullptr;
}