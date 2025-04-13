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

	if (StartMissionBtn) {
		StartMissionBtn->OnClicked.AddDynamic(this, &UMissionDescriptionWidget::OnStartMissionClicked);
		UUserWidgetHelpers::DesignButton(StartMissionBtn);
	}

	if (MissionTypeText) UUserWidgetHelpers::DesignText(MissionTypeText);
	if (MissionTitleText) UUserWidgetHelpers::DesignText(MissionTitleText);
	if (MissionDescriptionText) UUserWidgetHelpers::DesignText(MissionDescriptionText);
	if (MissionFluffText) UUserWidgetHelpers::DesignText(MissionFluffText);
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

	//auto missionDetailsManager = instance->GetMissionDetailsManager();

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

	auto missionFluffTextWidget = GetWidgetFromName("MissionFluffText");
	auto missionFluffText = (UTextBlock*)missionFluffTextWidget;
	if (missionFluffText) {
		missionFluffText->SetText(FText::FromString(missionDetails->GetFluffText()));
	}

	auto button = UUserWidgetHelpers::GetButtonFromWidget(this, "StartMissionBtn");
	if (button) {
		button->SetVisibility(missionDetails->GetIsCompleted() ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}

	auto completedImage = UUserWidgetHelpers::GetImageFromWidget(this, "CompletedImage");
	if (completedImage)
		completedImage->SetVisibility(missionDetails->GetIsCompleted() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (MissionImage)
		MissionImage->SetBrushFromTexture(UUserWidgetHelpers::GetRandomMissionImage());
}

void UMissionDescriptionWidget::OnStartMissionClicked() {

	auto instance = USvUtilities::GetGameInstance(GetWorld());
	if (!instance)
		return UDebugMessages::LogError(this, "failed to get game instance");

	if (!instance->GetCurrentGameDataManager() || !instance->GetCurrentGameDataManager()->GetCurrentGameData())
		return UDebugMessages::LogError(this, "failed to get current game data");

	auto currentGameData = instance->GetCurrentGameDataManager()->GetCurrentGameData();
	auto worldData = currentGameData->GetWorldData();
	auto currentLocation = worldData->GetCurrentLocation();

	currentGameData->StartNewMission(currentLocation->GetMissionDetails()->GetName(),
		currentLocation->GetMissionDetails()->GetMissionType(), currentLocation->GetMissionDetails());

	FName lName = FName("TopDownMap1");
	UGameplayStatics::OpenLevel(this, lName, true, "game=Class'/Script/SV.GameplayMode'?");
}