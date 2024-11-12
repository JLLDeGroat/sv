// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthAndStatusWidget.generated.h"

class UProgressBar;
class UTextBlock;

class UWidgetAnimation;
/**
 *
 */
UCLASS()
class SV_API UHealthAndStatusWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

	UProgressBar* GetHealthBar();
	UProgressBar* GetDynamicHealthBar();
	UTextBlock* GetNameTag();
	UTextBlock* GetDamageText();
	UTextBlock* GetStatusText();

	UWidgetAnimation* GetAnimateDamageIn() { return AnimateDamageIn; }
	UWidgetAnimation* GetAnimateDamageOut() { return AnimateDamageOut; }
	UWidgetAnimation* GetAnimateStatusIn() { return AnimateStatusIn; }
	UWidgetAnimation* GetAnimateStatusOut() { return AnimateStatusOut; }

protected:

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim)) UWidgetAnimation* AnimateDamageIn;
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim)) UWidgetAnimation* AnimateDamageOut;
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim)) UWidgetAnimation* AnimateStatusIn;
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim)) UWidgetAnimation* AnimateStatusOut;

	UPROPERTY(meta = (BindWidget)) UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget)) UProgressBar* DynamicHealthBar;
	UPROPERTY(meta = (BindWidget)) UTextBlock* NameTag;
	UPROPERTY(meta = (BindWidget)) UTextBlock* DamageText;
	UPROPERTY(meta = (BindWidget)) UTextBlock* StatusText;
};
