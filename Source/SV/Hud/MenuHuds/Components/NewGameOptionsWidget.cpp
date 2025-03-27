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

void UNewGameOptionsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ClassicBtn->OnClicked.AddDynamic(this, &UNewGameOptionsWidget::OnClassicGameClicked);

	BeginGameButton->OnClicked.AddDynamic(this, &UNewGameOptionsWidget::OnStartGameClicked);
	SetStartButtonVisibility(false);

	auto classicWidget = GetWidgetFromName("ClassicMode");
	auto classic = (UClassicGameModeWidget *)classicWidget;
	ClassicGameModeWidget = classic;

	ClassicGameModeWidget->SetVisibility(ESlateVisibility::Hidden);

	UUserWidgetHelpers::DesignButton(ClassicBtn);
	UUserWidgetHelpers::DesignButton(BeginGameButton);
	UUserWidgetHelpers::DesignButton(ComingSoonBtn);

	// UUserWidgetHelpers::DesignText(TitleText);
	// UUserWidgetHelpers::DesignText(DescriptionText);
	UUserWidgetHelpers::DesignText(EquipmentTitle);
	UUserWidgetHelpers::DesignText(ModifierTitle);
	UUserWidgetHelpers::DesignText(ModifierText);
	UUserWidgetHelpers::DesignText(GameModeGoalText);
	UUserWidgetHelpers::DesignText(GameModeGoalTitle);
	UUserWidgetHelpers::DesignText(EquipmentText);
	UUserWidgetHelpers::DesignText(GameModeTitle);
	UUserWidgetHelpers::DesignText(GameModeTitleText);

	ResetWidget();
}

void UNewGameOptionsWidget::ResetWidget()
{
	SetStartButtonVisibility(false);

	EquipmentTitle->SetText(FText::FromString(""));
	ModifierTitle->SetText(FText::FromString(""));
	ModifierText->SetText(FText::FromString(""));
	EquipmentText->SetText(FText::FromString(""));

	GameModeGoalText->SetText(FText::FromString(""));
	GameModeGoalTitle->SetText(FText::FromString(""));
	GameModeTitleText->SetText(FText::FromString(""));
}

void UNewGameOptionsWidget::OnClassicGameClicked()
{
	SetStartButtonVisibility(true);
	UpdateTitleAndDescriptions(EGameModeType::EMT_Classic);
}

void UNewGameOptionsWidget::OnStartGameClicked()
{
	// FName lName = FName("TopDownMap");
	// UGameplayStatics::OpenLevel(this, lName, true, "game=Class'/Script/SV.GameplayMode'?");

	// assuming classic game mode
	CurrentMapGrid = FMapGridData(12, 8);
	GenThread = NewObject<UClassicGameMapGenerationRunnable>()
					->InsertVariables(this, &CurrentMapGrid)
					->Initialise(GetWorld())
					->Begin();
}

void UNewGameOptionsWidget::SetStartButtonVisibility(bool val)
{
	BeginGameButton->SetVisibility(val ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UNewGameOptionsWidget::UpdateTitleAndDescriptions(EGameModeType gameMode)
{
	auto instance = USvUtilities::GetGameInstance(GetWorld());
	FGameTypeDescriptionItem item;
	instance->GetGameTypeDescription(EGameModeType::EMT_Classic, item);

	GameModeTitleText->SetText(FText::FromString(item.GetTitle()));
	GameModeGoalTitle->SetText(FText::FromString("Objectives"));

	EquipmentTitle->SetText(FText::FromString("Equipment"));
	ModifierTitle->SetText(FText::FromString("Modifiers"));

	GameModeGoalText->SetText(FText::FromString(item.GetModeGoal()));

	auto descriptions = item.GetModifierItems();
	FString totalDesc = "";
	for (int i = 0; i < descriptions.Num(); i++)
		totalDesc += descriptions[i] + "\r\n";
	ModifierText->SetText(FText::FromString(totalDesc));

	auto equipments = item.GetEquipmentItems();
	FString totalItems = "";
	for (int i = 0; i < equipments.Num(); i++)
		totalItems += equipments[i] + "\r\n";
	EquipmentText->SetText(FText::FromString(totalItems));
}

void UNewGameOptionsWidget::OnGenCompleted()
{
	FName lName = FName("WorldMap");
	UGameplayStatics::OpenLevel(this, lName, true, "game=Class'/Script/SV.WorldGameMode'?");
}