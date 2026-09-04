// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadPageWidget.h"

#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/TextBlock.h"
#include "Components/Pages/Squad/SquadListWidget.h"
#include "Components/Pages/Squad/CrewDetailsGridWidget.h"
#include "Components/Pages/Squad/CrewDADDetails.h"
#include "Components/Pages/Squad/SelectEquipmentListWidget.h"

void USquadPageWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Hidden);

	CrewDaDDetails->SetSquadPage(this);
	SquadListWidget->SetSquadPage(this);
	CrewDetailsGrid->SetSquadPage(this);
	SelectPrimaryList->SetSquadPage(this);
}

void USquadPageWidget::OpenPage()
{
	SetVisibility(ESlateVisibility::Visible);

	auto squadList = (USquadListWidget *)GetWidgetFromName("SquadListWidget");
	if (!squadList)
		return UDebugMessages::LogError(this, "Failed to get squadlist widget, not going further");

	squadList->SetupCrewList();
}

UCrewDADDetails *USquadPageWidget::GetCrewDADDetails()
{
	return CrewDaDDetails;
}