// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../../../../Enums/EConfigurationEnums.h"
#include "BaseTutorialWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class SV_API UBaseTutorialWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

protected:

	UPROPERTY(meta = (BindWidget)) UButton* ConfirmBtn;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TutDetails;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TutTitle;

	UFUNCTION() void TryShowWidget(ETutorials tutorial);
	UFUNCTION() void TryCompleteTutorial(ETutorials tutorial);

	UPROPERTY() ETutorials TutorialEnum;

	UFUNCTION() void OnConfirmClick();
};
