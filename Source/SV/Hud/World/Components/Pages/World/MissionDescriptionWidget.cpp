// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionDescriptionWidget.h"
#include "../../../../../Delegates/WorldDelegates.h"
#include "../../../../../Instance/SvGameInstance.h"
#include "../../../../../Instance/Managers/MissionDetailsManager.h"
#include "../../../../../Utilities/SvUtilities.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "../../../../../Enums/EWorldEnums.h"
#include "../../../../Helpers/EquipmentInventoryHelpers.h"
#include "Kismet/GameplayStatics.h"


void UMissionDescriptionWidget::NativeConstruct() {
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Hidden);

	auto worldDelegates = UWorldDelegates::GetInstance();
	if (worldDelegates) {
		worldDelegates->_OnWorldMovementComplete.AddDynamic(this, &UMissionDescriptionWidget::OnWorldMoveComplete);
	}

	auto startButton = UEquipmentInventoryHelpers::GetButtonFromWidget(this, "StartMissionBtn");
	if (startButton)
		startButton->OnClicked.AddDynamic(this, &UMissionDescriptionWidget::OnStartMissionClicked);
}

bool UMissionDescriptionWidget::GetMoveCompleted() const {
	return MoveCompleted;
}

void UMissionDescriptionWidget::OnWorldMoveComplete() {
	SetVisibility(ESlateVisibility::Visible);
	MoveCompleted = true;
	auto instance = USvUtilities::GetGameInstance(GetWorld());
	if (instance) {
		auto missionDetails = instance->GetMissionDetailsManager();
		auto missionName = missionDetails->GenerateMissionName();

		auto titleWidget = GetWidgetFromName("MissionTitle");
		auto title = (UTextBlock*)titleWidget;
		if (title) {
			title->SetText(FText::FromString(missionName));
		}

		auto missionType = (EMissionType)FMath::RandRange(1, 4);
		auto missionDescription = missionDetails->GetMissionDescription(missionType);

		auto descriptionWidget = GetWidgetFromName("MissionDescription");
		auto description = (UTextBlock*)descriptionWidget;
		if (description) {
			description->SetText(FText::FromString(missionDescription->GetDescription()));
		}

		auto nameWidget = GetWidgetFromName("MissionType");
		auto name = (UTextBlock*)nameWidget;
		if (name) {
			name->SetText(FText::FromString(missionDescription->GetMissionName()));
		}
	}
}

void UMissionDescriptionWidget::OnStartMissionClicked() {
	FName lName = FName("TopDownMap");
	UGameplayStatics::OpenLevel(this, lName, true, "game=Class'/Script/SV.GameplayMode'?");
}