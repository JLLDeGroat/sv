// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionFailedWidget.h"
#include "../../../Delegates/HudDelegates.h"
#include "Kismet/GameplayStatics.h"
#include "../../Helpers/UserWidgetHelpers.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/Button.h"

void UMissionFailedWidget::NativeConstruct() {
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Hidden);
	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "failed to get hud delegates");

	hudDelegates->_ShowMissionFailedWidget.AddDynamic(this, &UMissionFailedWidget::OnConsumeDelegate);

	auto mainMenuBtn = UUserWidgetHelpers::GetButtonFromWidget(this, "MainMenuBtn");
	if (!mainMenuBtn)
		return UDebugMessages::LogError(this, "failed to get maini menu button");

	mainMenuBtn->OnClicked.AddDynamic(this, &UMissionFailedWidget::MainMenuButtonClicked);
}

void UMissionFailedWidget::OnConsumeDelegate() {
	SetVisibility(ESlateVisibility::Visible);
}
void UMissionFailedWidget::MainMenuButtonClicked() {
	FName lName = FName("WorldMap");
	UGameplayStatics::OpenLevel(this, lName, true, "game=Class'/Script/SV.MenuGameMode'?");
}