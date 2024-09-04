// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionEndStatItemWidget.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/TextBlock.h"

void UMissionEndStatItemWidget::NativeConstruct() {
	Super::NativeConstruct();
}

void UMissionEndStatItemWidget::SetNameAndValue(FString name, FString value) {
	if (!StatName || !StatValue)
		return UDebugMessages::LogError(this, "failed to get statname or value text blocks wont set values");

	StatName->SetText(FText::FromString(name));
	StatValue->SetText(FText::FromString(value));
}