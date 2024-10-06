// Fill out your copyright notice in the Description page of Project Settings.


#include "NewGameOptionsWidget.h"
#include "Components/Button.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../Instance/SvGameInstance.h"
#include "../../../Utilities/SvUtilities.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "GameModes/ClassicGameModeWidget.h"
#include "../../../Runnables/ClassicGameMapGenerationRunnable.h"
#include "../../Helpers/UserWidgetHelpers.h"

void UNewGameOptionsWidget::NativeConstruct() {
	Super::NativeConstruct();

	ClassicBtn->OnClicked.AddDynamic(this, &UNewGameOptionsWidget::OnClassicGameClicked);

	BeginGameButton->OnClicked.AddDynamic(this, &UNewGameOptionsWidget::OnStartGameClicked);
	SetStartButtonVisibility(false);

	auto classicWidget = GetWidgetFromName("ClassicMode");
	auto classic = (UClassicGameModeWidget*)classicWidget;
	ClassicGameModeWidget = classic;

	ClassicGameModeWidget->SetVisibility(ESlateVisibility::Hidden);

	UUserWidgetHelpers::DesignButton(ClassicBtn);
	UUserWidgetHelpers::DesignButton(BeginGameButton);
	UUserWidgetHelpers::DesignButton(ComingSoonBtn);

	UUserWidgetHelpers::DesignText(TitleText);
	UUserWidgetHelpers::DesignText(DescriptionText);
}

void UNewGameOptionsWidget::ResetWidget() {
	SetStartButtonVisibility(false);

	TitleText->SetText(FText::FromString(""));
	DescriptionText->SetText(FText::FromString(""));
}

void UNewGameOptionsWidget::OnClassicGameClicked() {
	SetStartButtonVisibility(true);
	UpdateTitleAndDescriptions(EGameModeType::EMT_Classic);
}

void UNewGameOptionsWidget::OnStartGameClicked() {
	//FName lName = FName("TopDownMap");
	//UGameplayStatics::OpenLevel(this, lName, true, "game=Class'/Script/SV.GameplayMode'?");

	//assuming classic game mode
	CurrentMapGrid = FMapGridData(12, 8);
	GenThread = NewObject<UClassicGameMapGenerationRunnable>()
		->InsertVariables(this, &CurrentMapGrid)
		->Initialise(GetWorld())
		->Begin();
}

void UNewGameOptionsWidget::SetStartButtonVisibility(bool val) {
	BeginGameButton->SetVisibility(val ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UNewGameOptionsWidget::UpdateTitleAndDescriptions(EGameModeType gameMode) {
	auto instance = USvUtilities::GetGameInstance(GetWorld());
	FGameTypeDescriptionItem item;
	instance->GetGameTypeDescription(EGameModeType::EMT_Classic, item);


	TitleText->SetText(FText::FromString(item.GetTitle()));

	auto descriptions = item.GetDescriptions();
	FString totalDesc = "";
	for (int i = 0; i < descriptions.Num(); i++) {
		totalDesc += descriptions[i] + "\r\n";
	}
	DescriptionText->SetText(FText::FromString(totalDesc));
}

void UNewGameOptionsWidget::OnGenCompleted() {
	FName lName = FName("WorldMap");
	UGameplayStatics::OpenLevel(this, lName, true, "game=Class'/Script/SV.WorldGameMode'?");
}