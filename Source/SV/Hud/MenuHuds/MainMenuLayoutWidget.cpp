// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuLayoutWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Components/NewGameOptionsWidget.h"
#include "Components/GameModes/ClassicGameModeWidget.h"
#include "Components/NewGameOptionsWidget.h"
#include "Components/GridPanel.h"
#include "Components/CancelButtonWidget.h"

void UMainMenuLayoutWidget::NativeConstruct() {
	Super::NativeConstruct();

	auto quitWidget = GetWidgetFromName("QuitButton");
	auto startWidget = GetWidgetFromName("startButton");

	auto mainMenuWidget = GetWidgetFromName("MainMenuWidget");
	MainMenuGrid = (UGridPanel*)mainMenuWidget;

	if (quitWidget->IsA<UButton>()) {
		auto btn = (UButton*)quitWidget;
		btn->OnClicked.AddDynamic(this, &UMainMenuLayoutWidget::OnQuitClicked);
	}

	if (startWidget->IsA<UButton>()) {
		auto btn = (UButton*)startWidget;
		btn->OnClicked.AddDynamic(this, &UMainMenuLayoutWidget::GoToNewGameOptions);
	}

	auto newGameOptions = GetWidgetFromName("NewGameOptions");
	auto classicGameOptions = GetWidgetFromName("ClassicGameModeOptions");

	NewGameOptionsWidget = (UNewGameOptionsWidget*)newGameOptions;

	if (NewGameOptionsWidget) NewGameOptionsWidget->SetVisibility(ESlateVisibility::Hidden);

	auto cancelWidget = GetWidgetFromName("CancelButton");
	CancelButtonWidget = (UCancelButtonWidget*)cancelWidget;

	if (CancelButtonWidget) {
		auto cancelButtonWidget = CancelButtonWidget->GetWidgetFromName("CancelButton");
		auto cancelButton = (UButton*)cancelButtonWidget;
		if (cancelButton) cancelButton->OnClicked.AddDynamic(this, &UMainMenuLayoutWidget::GoToMainMenu);
	}

	if (CancelButtonWidget) CancelButtonWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UMainMenuLayoutWidget::OnStartClicked() {
	//FName lName = FName("TopDownMap");
	//UGameplayStatics::OpenLevel(this, lName, true, "game=Class'/Script/SV.GameplayMode'?");
}
void UMainMenuLayoutWidget::OnQuitClicked() {
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
	//FGenericPlatformMisc::RequestExit(false);
}

void UMainMenuLayoutWidget::GoToMainMenu() {
	NewGameOptionsWidget->SetVisibility(ESlateVisibility::Hidden);
	CancelButtonWidget->SetVisibility(ESlateVisibility::Hidden);

	MainMenuGrid->SetVisibility(ESlateVisibility::Visible);
}
void UMainMenuLayoutWidget::GoToNewGameOptions() {
	MainMenuGrid->SetVisibility(ESlateVisibility::Hidden);

	NewGameOptionsWidget->SetVisibility(ESlateVisibility::Visible);
	CancelButtonWidget->SetVisibility(ESlateVisibility::Visible);

	NewGameOptionsWidget->ResetWidget();
}