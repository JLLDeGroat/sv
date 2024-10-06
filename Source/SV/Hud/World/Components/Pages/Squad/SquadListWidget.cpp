// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadListWidget.h"
#include "../../../../../Utilities/SvUtilities.h"
#include "../../../../../Instance/SvGameInstance.h"
#include "../../../../../Instance/Managers/CurrentGameDataManager.h"
#include "../../../../../Delegates/TutorialDelegates.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "CrewDetailsGridWidget.h"
#include "../../../../Helpers/UserWidgetHelpers.h"

void USquadListWidget::NativeConstruct() {
	Super::NativeConstruct();

	if (CrewBtn1) {
		CrewBtn1->OnClicked.AddDynamic(this, &USquadListWidget::Crew1Clicked);
		UUserWidgetHelpers::DesignButton(CrewBtn1);
	}
	if (CrewBtn2) {
		CrewBtn2->OnClicked.AddDynamic(this, &USquadListWidget::Crew2Clicked);
		UUserWidgetHelpers::DesignButton(CrewBtn2);
	}
	if (CrewBtn3) {
		CrewBtn3->OnClicked.AddDynamic(this, &USquadListWidget::Crew3Clicked);
		UUserWidgetHelpers::DesignButton(CrewBtn3);
	}
	if (CrewBtn4) {
		CrewBtn4->OnClicked.AddDynamic(this, &USquadListWidget::Crew4Clicked);
		UUserWidgetHelpers::DesignButton(CrewBtn4);
	}
	if (CrewBtn5) {
		CrewBtn5->OnClicked.AddDynamic(this, &USquadListWidget::Crew5Clicked);
		UUserWidgetHelpers::DesignButton(CrewBtn5);
	}
	if (CrewBtn6) {
		CrewBtn6->OnClicked.AddDynamic(this, &USquadListWidget::Crew6Clicked);
		UUserWidgetHelpers::DesignButton(CrewBtn6);
	}

}

void USquadListWidget::SetupCrewList() {
	auto instance = USvUtilities::GetGameInstance(GetWorld());

	if (!instance || !instance->GetCurrentGameDataManager())
		return UDebugMessages::LogError(this, "failed to get instance/gamedatamanager");

	auto gameData = instance->GetCurrentGameDataManager();
	auto crew = gameData->GetEntireCrew();

	for (int i = 0; i < 6; i++) {
		if (crew.Num() > i) {
			auto text = (UTextBlock*)GetWidgetFromName(FName("CrewText" + FString::SanitizeFloat(i + 1, 0)));
			if (text)
				text->SetText(FText::FromString(crew[i].GetName()));
		}
		else {
			auto btn = (UButton*)GetWidgetFromName(FName("CrewBtn" + FString::SanitizeFloat(i + 1, 0)));
			if (btn)
				btn->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void USquadListWidget::Crew1Clicked() {
	ShowCrewDetails(0);
}
void USquadListWidget::Crew2Clicked() {
	ShowCrewDetails(1);
}
void USquadListWidget::Crew3Clicked() {
	ShowCrewDetails(2);
}
void USquadListWidget::Crew4Clicked() {
	ShowCrewDetails(3);
}
void USquadListWidget::Crew5Clicked() {
	ShowCrewDetails(4);
}
void USquadListWidget::Crew6Clicked() {
	ShowCrewDetails(5);
}

void USquadListWidget::ShowCrewDetails(int crewClicked) {
	auto tutorialDelegates = UTutorialDelegates::GetInstance();
	if (!tutorialDelegates)
		return UDebugMessages::LogError(this, "failed to get tutorial delegates");

	tutorialDelegates->_OnTryShowTutorial.Broadcast(ETutorials::T_EquipmentSelect);

	UDebugMessages::LogDisplay(this, "crew clicked " + FString::SanitizeFloat(crewClicked));

	auto parentWidget = (UUserWidget*)GetOuter()->GetOuter();
	auto detailsGrid = (UCrewDetailsGridWidget*)parentWidget->GetWidgetFromName("CrewDetailsGrid");

	auto instance = USvUtilities::GetGameInstance(GetWorld());
	auto gameDataManager = instance->GetCurrentGameDataManager();
	auto gameData = gameDataManager->GetCurrentGameData();
	auto crewMemberData = gameData->GetCrew()[crewClicked];

	if (detailsGrid) {
		detailsGrid->SetVisibility(ESlateVisibility::Visible);
		detailsGrid->InitialiseGridForCrewMember(crewMemberData.GetId());
	}
}