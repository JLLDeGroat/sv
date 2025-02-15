// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseTutorialWidget.h"
#include "../../../../Helpers/UserWidgetHelpers.h"
#include "../../../../../Delegates/TutorialDelegates.h"
#include "../../../../../Utilities/SvUtilities.h"
#include "../../../../../Data/Game/FCurrentGameData.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

void UBaseTutorialWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (TutTitle)
		UUserWidgetHelpers::DesignText(TutTitle);
	if (TutDetails)
		UUserWidgetHelpers::DesignText(TutDetails, 18);
	if (ConfirmBtn)
	{
		ConfirmBtn->OnClicked.AddDynamic(this, &UBaseTutorialWidget::OnConfirmClick);
		UUserWidgetHelpers::DesignButton(ConfirmBtn);
		UUserWidgetHelpers::SetButtonText(ConfirmBtn, "Confirm");
	}
	auto tutorialDelegates = UTutorialDelegates::GetInstance();
	if (!tutorialDelegates)
		return UDebugMessages::LogError(this, "failed to get tutorial delegates");

	tutorialDelegates->_OnCompleteTutorial.AddDynamic(this, &UBaseTutorialWidget::TryCompleteTutorial);
	tutorialDelegates->_OnTryShowTutorial.AddDynamic(this, &UBaseTutorialWidget::TryShowWidget);

	if (TutorialBorder)
	{
		FSlateBrush brush;
		brush.TintColor = FSlateColor(FLinearColor(0, 0, 0, 1.0f)); // 50% opacity
		TutorialBorder->SetBrush(brush);
	}
}

void UBaseTutorialWidget::TryShowWidget(ETutorials tutorial)
{
	if (tutorial != TutorialEnum)
		return;

	auto gameData = USvUtilities::GetCurrentGameData(GetWorld());
	if (!gameData)
		return UDebugMessages::LogError(this, "failed to get gameData");

	auto tutorialData = gameData->GetTutorialData();

	if (!tutorialData->GetCompletedTutorial(tutorial))
		SetVisibility(ESlateVisibility::Visible);
}

void UBaseTutorialWidget::OnConfirmClick()
{
	auto gameData = USvUtilities::GetCurrentGameData(GetWorld());
	if (!gameData)
		return UDebugMessages::LogError(this, "failed to get gameData");

	auto tutorialData = gameData->GetTutorialData();
	tutorialData->SetCompletedTurorial(TutorialEnum);

	SetVisibility(ESlateVisibility::Hidden);
}

void UBaseTutorialWidget::TryCompleteTutorial(ETutorials tutorial)
{
	if (tutorial == TutorialEnum)
		OnConfirmClick();
}