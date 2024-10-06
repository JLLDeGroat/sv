// Fill out your copyright notice in the Description page of Project Settings.


#include "NavigatingWorldTutorialWidget.h"
#include "../../../Helpers/UserWidgetHelpers.h"
#include "../../../../Instance/SvGameInstance.h"
#include "../../../../Utilities/SvUtilities.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../../Data/Game/FCurrentGameData.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "../../../../Delegates/TutorialDelegates.h"

void UNavigatingWorldTutorialWidget::NativeConstruct() {
	Super::NativeConstruct();
	TutorialEnum = ETutorials::T_WorldNavigation;
	SetVisibility(ESlateVisibility::Hidden);
	TryShowWidget(ETutorials::T_WorldNavigation);
}