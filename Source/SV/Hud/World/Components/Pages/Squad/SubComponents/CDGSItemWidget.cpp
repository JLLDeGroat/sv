// Fill out your copyright notice in the Description page of Project Settings.

#include "CDGSItemWidget.h"
#include "../../../../../Helpers/UserWidgetHelpers.h"
#include "Components/TextBlock.h"

void UCDGSItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StatTitle)
		UUserWidgetHelpers::DesignText(StatTitle);

	if (StatValue)
		UUserWidgetHelpers::DesignText(StatTitle);
}

void UCDGSItemWidget::SetTitle(FString title)
{
	StatTitle->SetText(FText::FromString(title));
}
void UCDGSItemWidget::SetValue(int value)
{
	StatValue->SetText(FText::FromString(FString::SanitizeFloat(value)));
}