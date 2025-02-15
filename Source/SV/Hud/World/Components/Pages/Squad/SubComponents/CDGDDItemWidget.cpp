// Fill out your copyright notice in the Description page of Project Settings.

#include "CDGDDItemWidget.h"
#include "../../../../../Helpers/UserWidgetHelpers.h"
#include "../../../../../../Utilities/SvUtilities.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UCDGDDItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (DDTitle)
		UUserWidgetHelpers::DesignText(DDTitle);

	if (DDDescription)
		UUserWidgetHelpers::DesignText(DDDescription, 18);
}

void UCDGDDItemWidget::SetTitle(FString str)
{
	DDTitle->SetText(FText::FromString(str));
}
void UCDGDDItemWidget::SetDescription(FString str)
{
	DDDescription->SetText(FText::FromString(str));
}

void UCDGDDItemWidget::SetImageAsDirective()
{
	auto image = USvUtilities::GetTexture("/Script/Engine.Texture2D'/Game/Images/UI/DirectiveIcon.DirectiveIcon'");
	if (image)
		DDImage->SetBrushFromTexture(image);
}
void UCDGDDItemWidget::SetImageAsDeviation()
{
	auto image = USvUtilities::GetTexture("/Script/Engine.Texture2D'/Game/Images/UI/DeviationIcon.DeviationIcon'");
	if (image)
		DDImage->SetBrushFromTexture(image);
}