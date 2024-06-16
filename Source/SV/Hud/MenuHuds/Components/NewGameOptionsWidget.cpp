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

void UNewGameOptionsWidget::NativeConstruct() {
	Super::NativeConstruct();

	auto classicGameButton = GetClassicGameButton();
	classicGameButton->OnClicked.AddDynamic(this, &UNewGameOptionsWidget::OnClassicGameClicked);

	auto startButton = GetBeginGameButton();
	startButton->OnClicked.AddDynamic(this, &UNewGameOptionsWidget::OnStartGameClicked);
	SetStartButtonVisibility(false);

	auto classicWidget = GetWidgetFromName("ClassicMode");
	auto classic = (UClassicGameModeWidget*)classicWidget;
	ClassicGameModeWidget = classic;

	ClassicGameModeWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UNewGameOptionsWidget::ResetWidget() {
	SetStartButtonVisibility(false);

	auto descBlock = GetDescriptionBlock();
	auto titleBlock = GetTitleBlock();

	titleBlock->SetText(FText::FromString(""));
	descBlock->SetText(FText::FromString(""));
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
	auto beginGameButton = GetBeginGameButton();
	beginGameButton->SetVisibility(val ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UNewGameOptionsWidget::UpdateTitleAndDescriptions(EGameModeType gameMode) {
	auto instance = USvUtilities::GetGameInstance(GetWorld());
	FGameTypeDescriptionItem item;
	instance->GetGameTypeDescription(EGameModeType::EMT_Classic, item);


	auto titleText = GetTitleBlock();
	auto descriptionText = GetDescriptionBlock();

	titleText->SetText(FText::FromString(item.GetTitle()));

	auto descriptions = item.GetDescriptions();
	FString totalDesc = "";
	for (int i = 0; i < descriptions.Num(); i++) {
		totalDesc += descriptions[i] + "\r\n";
	}
	descriptionText->SetText(FText::FromString(totalDesc));
}

UButton* UNewGameOptionsWidget::GetBeginGameButton() {
	auto widget = GetWidgetFromName("BeginGameButton");
	auto button = (UButton*)widget;
	return button;
}
UButton* UNewGameOptionsWidget::GetClassicGameButton() {
	auto widget = GetWidgetFromName("ClassicBtn");
	auto button = (UButton*)widget;
	return button;
}
UTextBlock* UNewGameOptionsWidget::GetTitleBlock() {
	auto widget = GetWidgetFromName("TitleText");
	auto block = (UTextBlock*)widget;
	return block;
}
UTextBlock* UNewGameOptionsWidget::GetDescriptionBlock() {
	auto widget = GetWidgetFromName("DescriptionText");
	auto block = (UTextBlock*)widget;
	return block;
}

void UNewGameOptionsWidget::OnGenCompleted() {
	FName lName = FName("WorldMap");
	UGameplayStatics::OpenLevel(this, lName, true, "game=Class'/Script/SV.WorldGameMode'?");
}