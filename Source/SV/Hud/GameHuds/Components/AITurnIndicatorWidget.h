// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AITurnIndicatorWidget.generated.h"

class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class SV_API UAITurnIndicatorWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

protected:

	UFUNCTION() void SetIndicatorVisibility(bool bVisible);
	UFUNCTION() void OnAnimateOutFinish();

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim)) UWidgetAnimation* AnimateIn;
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim)) UWidgetAnimation* AnimateOut;
	
};
