// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadListWidget.h"
#include "../../../../../Utilities/SvUtilities.h"
#include "../../../../../Instance/SvGameInstance.h"
#include "../../../../../Instance/Managers/CurrentGameDataManager.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "CrewDetailsGridWidget.h"

void USquadListWidget::NativeConstruct() {
	Super::NativeConstruct();

	auto btn1 = (UButton*)GetWidgetFromName("CrewBtn1");
	auto btn2 = (UButton*)GetWidgetFromName("CrewBtn2");
	auto btn3 = (UButton*)GetWidgetFromName("CrewBtn3");
	auto btn4 = (UButton*)GetWidgetFromName("CrewBtn4");
	auto btn5 = (UButton*)GetWidgetFromName("CrewBtn5");
	auto btn6 = (UButton*)GetWidgetFromName("CrewBtn6");

	if (btn1) btn1->OnClicked.AddDynamic(this, &USquadListWidget::Crew1Clicked);
	if (btn2) btn2->OnClicked.AddDynamic(this, &USquadListWidget::Crew2Clicked);
	if (btn3) btn3->OnClicked.AddDynamic(this, &USquadListWidget::Crew3Clicked);
	if (btn4) btn4->OnClicked.AddDynamic(this, &USquadListWidget::Crew4Clicked);
	if (btn5) btn5->OnClicked.AddDynamic(this, &USquadListWidget::Crew5Clicked);
	if (btn6) btn6->OnClicked.AddDynamic(this, &USquadListWidget::Crew6Clicked);

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