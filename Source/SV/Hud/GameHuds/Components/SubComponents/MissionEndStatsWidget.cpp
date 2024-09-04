// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionEndStatsWidget.h"
#include "../../../../Utilities/SvUtilities.h"
#include "../../../../Data/Game/Mission/FMissionDetails.h"
#include "../../../../Data/Game/Mission/FMissionStats.h"
#include "../../../../Data/Game/FCurrentGameData.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../../Delegates/HudDelegates.h"
#include "../../../Helpers/UserWidgetHelpers.h"

#include "MissionEndStatItemWidget.h"
#include "Components/VerticalBox.h"

void UMissionEndStatsWidget::NativeConstruct() {
	Super::NativeConstruct();

	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		UDebugMessages::LogError(this, "failed to get hud Delegates");
	else
		hudDelegates->_ShowMissionCompleteWidget.AddDynamic(this, &UMissionEndStatsWidget::OnShow);
}


void UMissionEndStatsWidget::OnShow() {
	auto currentMission = USvUtilities::GetCurrentGameData(GetWorld());

	if (currentMission) {
		auto currentMissionStats = currentMission
			->GetCurrentMission()
			->GetMissionDetails()
			->GetMissionStats();

		if (!StatBox)
			return UDebugMessages::LogError(this, "failed to get stat box, not showing stats");

		StatBox->ClearChildren();

		CreateStatItem("Kills", FString::SanitizeFloat(currentMissionStats->GetKilledAmount(), 0));
	}
}

void UMissionEndStatsWidget::CreateStatItem(FString statName, FString statValue) {
	auto statItem = CreateStatItemWidget();
	if (!statItem)
		return UDebugMessages::LogError(this, "failed to create stat item widget");

	StatBox->AddChild(statItem);
	statItem->SetNameAndValue(statName, statValue);
}

UMissionEndStatItemWidget* UMissionEndStatsWidget::CreateStatItemWidget() {
	FSoftClassPath hudUIRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/Components/SubComponents/MissionEndStatItem_Bp.MissionEndStatItem_Bp_C'"));
	if (UClass* hudUIWidgetClass = hudUIRef.TryLoadClass<UMissionEndStatItemWidget>())
		return CreateWidget<UMissionEndStatItemWidget>(this, hudUIWidgetClass);

	return nullptr;
}