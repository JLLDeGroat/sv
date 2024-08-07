// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionCompleteWidget.h"
#include "../../../Delegates/HudDelegates.h"
#include "../../Helpers/UserWidgetHelpers.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/Button.h"
#include "../../Helpers/UserWidgetHelpers.h"
#include "Kismet/GameplayStatics.h"

void UMissionCompleteWidget::NativeConstruct() {
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Hidden);
	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "failed to get hud delegates");

	hudDelegates->_ShowMissionCompleteWidget.AddDynamic(this, &UMissionCompleteWidget::OnConsumeDelegate);

	auto continueBtn = UUserWidgetHelpers::GetButtonFromWidget(this, "ContinueBtn");
	if (!continueBtn)
		return UDebugMessages::LogError(this, "faile to get continue button");

	continueBtn->OnClicked.AddDynamic(this, &UMissionCompleteWidget::OnContinueClicked);
}

void UMissionCompleteWidget::OnConsumeDelegate() {
	SetVisibility(ESlateVisibility::Visible);
}

void UMissionCompleteWidget::OnContinueClicked() {
	FName lName = FName("WorldMap");
	UGameplayStatics::OpenLevel(this, lName, true, "game=Class'/Script/SV.WorldGameMode'?");
}