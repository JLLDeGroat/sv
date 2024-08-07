// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionDescriptionWidget.h"
#include "../../../../../Delegates/WorldDelegates.h"
#include "../../../../../Instance/SvGameInstance.h"
#include "../../../../../Instance/Managers/MissionDetailsManager.h"
#include "../../../../../Instance/SvGameInstance.h"
#include "../../../../../Instance/Managers/CurrentGameDataManager.h"
#include "../../../../../Utilities/SvUtilities.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "../../../../../Enums/EWorldEnums.h"
#include "../../../../Helpers/UserWidgetHelpers.h"
#include "Kismet/GameplayStatics.h"


void UMissionDescriptionWidget::NativeConstruct() {
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Hidden);

	auto worldDelegates = UWorldDelegates::GetInstance();
	if (worldDelegates) {
		worldDelegates->_OnWorldMovementComplete.AddDynamic(this, &UMissionDescriptionWidget::OnWorldMoveComplete);
	}

	auto startButton = UUserWidgetHelpers::GetButtonFromWidget(this, "StartMissionBtn");
	if (startButton)
		startButton->OnClicked.AddDynamic(this, &UMissionDescriptionWidget::OnStartMissionClicked);
}

bool UMissionDescriptionWidget::GetMoveCompleted() const {
	return MoveCompleted;
}

void UMissionDescriptionWidget::OnWorldMoveComplete(FVector2D MovedToLocation) {
	SetVisibility(ESlateVisibility::Visible);
	MoveCompleted = true;

	//this should get the mission
	UDebugMessages::LogError(this, "TODO this should be gotton from the world data");
	auto instance = USvUtilities::GetGameInstance(GetWorld());

	if (!instance)
		return UDebugMessages::LogError(this, "failed to get game instance");

	if (!instance->GetCurrentGameDataManager() || !instance->GetCurrentGameDataManager()->GetCurrentGameData())
		return UDebugMessages::LogError(this, "failed to get current data manager");

	auto currentGameData = instance->GetCurrentGameDataManager()->GetCurrentGameData();
	auto worldData = currentGameData->GetWorldData();

	auto thisLocation = worldData->GetWorldLocationData(MovedToLocation);
	auto missionDetails = thisLocation->GetMissionDetails();

	MissionName = missionDetails->GetName();
	MissionType = (uint8)missionDetails->GetMissionType();

	auto missionDetailsManager = instance->GetMissionDetailsManager();

	auto titleWidget = GetWidgetFromName("MissionTitleText");
	auto title = (UTextBlock*)titleWidget;
	if (title) {
		title->SetText(FText::FromString(MissionName));
	}
	auto nameWidget = GetWidgetFromName("MissionTypeText");
	auto name = (UTextBlock*)nameWidget;
	if (name) {
		name->SetText(FText::FromString(missionDetails->GetMissionTypeDescription()));
	}

	auto descriptionWidget = GetWidgetFromName("MissionDescriptionText");
	auto description = (UTextBlock*)descriptionWidget;
	if (description) {
		description->SetText(FText::FromString(missionDetails->GetDescription()));
	}

	auto button = UUserWidgetHelpers::GetButtonFromWidget(this, "StartMissionBtn");
	if (button) {
		button->SetVisibility(missionDetails->GetIsCompleted() ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}

	auto completedImage = UUserWidgetHelpers::GetImageFromWidget(this, "CompletedImage");
	if (completedImage)
		completedImage->SetVisibility(missionDetails->GetIsCompleted() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UMissionDescriptionWidget::OnStartMissionClicked() {

	auto instance = USvUtilities::GetGameInstance(GetWorld());
	if (!instance)
		return UDebugMessages::LogError(this, "failed to get game instance");

	if (!instance->GetCurrentGameDataManager() || !instance->GetCurrentGameDataManager()->GetCurrentGameData())
		return UDebugMessages::LogError(this, "failed to get current game data");

	auto currentGameData = instance->GetCurrentGameDataManager()->GetCurrentGameData();
	currentGameData->StartNewMission(MissionName, (EMissionType)MissionType);

	FName lName = FName("TopDownMap");
	UGameplayStatics::OpenLevel(this, lName, true, "game=Class'/Script/SV.GameplayMode'?");
}