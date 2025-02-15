// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CDGDDItemWidget.generated.h"

class UTextBlock;
class UImage;
/**
 *
 */
UCLASS()
class SV_API UCDGDDItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	void SetTitle(FString str);
	void SetDescription(FString str);

	void SetImageAsDirective();
	void SetImageAsDeviation();

protected:

	UPROPERTY(meta = (BindWidget))
    UTextBlock *DDTitle;
	UPROPERTY(meta = (BindWidget))
    UTextBlock *DDDescription;
	UPROPERTY(meta = (BindWidget))
    UImage* DDImage;
};
