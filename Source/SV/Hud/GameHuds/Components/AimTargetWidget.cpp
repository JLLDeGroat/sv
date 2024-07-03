// Fill out your copyright notice in the Description page of Project Settings.


#include "AimTargetWidget.h"
#include "../../../Delegates/HudDelegates.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

void UAimTargetWidget::NativeConstruct() {
	Super::NativeConstruct();
	auto hudDelegates = UHudDelegates::GetInstance();

	if (!hudDelegates)
		return UDebugMessages::LogError(this, "could not find hud instances");

	hudDelegates->_AimTargetVisibility.AddDynamic(this, &UAimTargetWidget::SetAimTargetVisibility);

	SetAimTargetVisibility(false);
}

void UAimTargetWidget::SetAimTargetVisibility(bool val) {
	auto vis = val ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	SetVisibility(vis);
}