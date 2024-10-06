// Fill out your copyright notice in the Description page of Project Settings.


#include "SuppliesTutorialWidget.h"

void USuppliesTutorialWidget::NativeConstruct() {
	Super::NativeConstruct();
	TutorialEnum = ETutorials::T_Supplies;
	SetVisibility(ESlateVisibility::Hidden);
}