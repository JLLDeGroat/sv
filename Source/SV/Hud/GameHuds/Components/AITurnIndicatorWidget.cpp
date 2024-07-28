// Fill out your copyright notice in the Description page of Project Settings.


#include "AITurnIndicatorWidget.h"
#include "Animation/WidgetAnimation.h"
#include "../../../Delegates/AIDelegates.h"

void UAITurnIndicatorWidget::NativeConstruct() {
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Hidden);

	auto aiDelegates = UAIDelegates::GetInstance();
	if (aiDelegates)
		aiDelegates->_AiTurnIndicatorVisibility.AddDynamic(this, &UAITurnIndicatorWidget::SetIndicatorVisibility);

	FWidgetAnimationDynamicEvent MyAnimationEvent;
	MyAnimationEvent.BindDynamic(this, &UAITurnIndicatorWidget::OnAnimateOutFinish);
	BindToAnimationFinished(AnimateOut, MyAnimationEvent);
}

void UAITurnIndicatorWidget::SetIndicatorVisibility(bool bVisible) {
	if (bVisible) {
		SetVisibility(ESlateVisibility::Visible);
		PlayAnimationForward(AnimateIn);
	}
	else {
		PlayAnimationForward(AnimateOut);
	}
}

void UAITurnIndicatorWidget::OnAnimateOutFinish() {
	SetVisibility(ESlateVisibility::Hidden);
}