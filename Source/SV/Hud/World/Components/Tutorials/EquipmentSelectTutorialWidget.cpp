// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSelectTutorialWidget.h"

void UEquipmentSelectTutorialWidget::NativeConstruct() {
	Super::NativeConstruct();
	TutorialEnum = ETutorials::T_EquipmentSelect;
	SetVisibility(ESlateVisibility::Hidden);
}