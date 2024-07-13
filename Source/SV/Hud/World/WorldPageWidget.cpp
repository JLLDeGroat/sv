// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldPageWidget.h"
#include "Components/Pages/World/MissionDescriptionWidget.h"

void UWorldPageWidget::NativeConstruct() {
	Super::NativeConstruct();
}
void UWorldPageWidget::OpenPage() {
	SetVisibility(ESlateVisibility::Visible);
	auto missionDescription = (UMissionDescriptionWidget*)GetWidgetFromName("MissionDescription");

	if (missionDescription && missionDescription->GetMoveCompleted())
		missionDescription->SetVisibility(ESlateVisibility::Visible);
}