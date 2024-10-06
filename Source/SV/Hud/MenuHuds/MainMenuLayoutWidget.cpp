// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuLayoutWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Components/NewGameOptionsWidget.h"
#include "Components/GameModes/ClassicGameModeWidget.h"
#include "Components/NewGameOptionsWidget.h"
#include "Components/GridPanel.h"
#include "Components/CancelButtonWidget.h"
#include "../Helpers/UserWidgetHelpers.h"

void UMainMenuLayoutWidget::NativeConstruct() {
	Super::NativeConstruct();

	auto quitWidget = GetWidgetFromName("QuitButton");
	auto startWidget = GetWidgetFromName("StartButton");

	auto mainMenuWidget = GetWidgetFromName("MainMenuWidget");

	if (QuitButton) {
		QuitButton->OnClicked.AddDynamic(this, &UMainMenuLayoutWidget::OnQuitClicked);
		UUserWidgetHelpers::DesignButton(QuitButton);
	}

	if (StartButton) {
		StartButton->OnClicked.AddDynamic(this, &UMainMenuLayoutWidget::GoToNewGameOptions);
		UUserWidgetHelpers::DesignButton(StartButton);
	}

	NewGameOptionsWidget = (UNewGameOptionsWidget*)GetWidgetFromName("NewGameOptions");
	auto classicGameOptions = GetWidgetFromName("ClassicGameModeOptions");

	if (NewGameOptionsWidget) NewGameOptionsWidget->SetVisibility(ESlateVisibility::Hidden);

	auto cancelWidget = GetWidgetFromName("CancelButton");

	if (CancelButton) {
		CancelButton->SetVisibility(ESlateVisibility::Hidden);

		auto cancelButtonWidget = CancelButton->GetWidgetFromName("CancelButton");
		auto cancelButton = (UButton*)cancelButtonWidget;
		if (cancelButton) {
			UUserWidgetHelpers::DesignButton(cancelButton);
			cancelButton->OnClicked.AddDynamic(this, &UMainMenuLayoutWidget::GoToMainMenu);
		}
	}
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
	CancelButton->SetVisibility(ESlateVisibility::Hidden);

	MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
}
void UMainMenuLayoutWidget::GoToNewGameOptions() {
	MainMenuWidget->SetVisibility(ESlateVisibility::Hidden);

	NewGameOptionsWidget->SetVisibility(ESlateVisibility::Visible);
	CancelButton->SetVisibility(ESlateVisibility::Visible);

	NewGameOptionsWidget->ResetWidget();
}