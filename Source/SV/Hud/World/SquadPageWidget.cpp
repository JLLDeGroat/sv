// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadPageWidget.h"

#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/TextBlock.h"
#include "Components/Pages/Squad/SquadListWidget.h"
#include "Components/Pages/Squad/CrewDetailsGridWidget.h"

void USquadPageWidget::NativeConstruct() {
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Hidden);
}

void USquadPageWidget::OpenPage() {
	SetVisibility(ESlateVisibility::Visible);

	auto squadList = (USquadListWidget*)GetWidgetFromName("SquadListWidget");
	if (!squadList) 
		return UDebugMessages::LogError(this, "Failed to get squadlist widget, not going further");

	squadList->SetupCrewList();
}