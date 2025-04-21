// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelLoadingScreenWidget.h"
#include "../../Delegates/HudDelegates.h"
#include "../../Delegates/CharacterDelegates.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void ULevelLoadingScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto hudDelegates = UHudDelegates::GetInstance();
	if (hudDelegates)
	{
		hudDelegates->_LevelLoadingUpdate.AddDynamic(this, &ULevelLoadingScreenWidget::OnLevelLoadingUpdate);
		hudDelegates->_LevelLoadingSetWaitForFog.AddDynamic(this, &ULevelLoadingScreenWidget::OnLevelLoadingWaitForFog);
		hudDelegates->_LevelLoadingSetWaitForFogComplete.AddDynamic(this, &ULevelLoadingScreenWidget::OnLevelLoadingSetWaitForFogComplete);
	}
	LoadingText->SetText(FText::FromString(""));
	LoadingProgress->SetPercent(0);
}

void ULevelLoadingScreenWidget::OnLevelLoadingUpdate(FString Msg, float Percentage)
{
	if (Percentage >= 1 && ((bWaitForFog && bFogIsComplete) || !bWaitForFog))
	{
		SetVisibility(ESlateVisibility::Hidden);

		// double check that the fog handler works
		auto characterDelegates = UCharacterDelegates::GetInstance();
		if (characterDelegates)
		{
			characterDelegates->_OnFogGenerationComplete.Broadcast();
		}
	}
	else if (Percentage >= 1)
	{
		Msg = "Finishing up";
	}

	CurrentPercentage = Percentage;
	CurrentMessage = Msg;

	LoadingText->SetText(FText::FromString(Msg));
	LoadingProgress->SetPercent(Percentage);
}

void ULevelLoadingScreenWidget::OnLevelLoadingWaitForFog()
{
	bWaitForFog = true;
}

void ULevelLoadingScreenWidget::OnLevelLoadingSetWaitForFogComplete()
{
	bFogIsComplete = true;
	OnLevelLoadingUpdate(CurrentMessage, CurrentPercentage);
}