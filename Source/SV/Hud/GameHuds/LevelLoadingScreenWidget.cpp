// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelLoadingScreenWidget.h"
#include "../../Delegates/HudDelegates.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void ULevelLoadingScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto hudDelegates = UHudDelegates::GetInstance();
	if (hudDelegates)
		hudDelegates->_LevelLoadingUpdate.AddDynamic(this, &ULevelLoadingScreenWidget::OnLevelLoadingUpdate);

	LoadingText->SetText(FText::FromString(""));
	LoadingProgress->SetPercent(0);
}

void ULevelLoadingScreenWidget::OnLevelLoadingUpdate(FString Msg, float Percentage)
{
	LoadingText->SetText(FText::FromString(Msg));
	LoadingProgress->SetPercent(Percentage);

	if (Percentage >= 1)
		SetVisibility(ESlateVisibility::Hidden);
}