// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWorldWidget.h"

void UMainWorldWidget::NativeConstruct() {
	Super::NativeConstruct();

	auto widget = GetWidgetFromName("MissionDescriptionWidget");
	DescriptionWidget = (UMissionDescriptionWidget*)widget;
}