// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuLayoutWidget.generated.h"

class UNewGameOptionsWidget;
class UCancelButtonWidget;
class UGridPanel;

/**
 *
 */
UCLASS()
class SV_API UMainMenuLayoutWidget : public UUserWidget
{
	GENERATED_BODY()

public:

protected:


	virtual void NativeConstruct() override;

	UFUNCTION() void OnStartClicked();
	UFUNCTION() void OnQuitClicked();

	UFUNCTION() void GoToMainMenu();
	UFUNCTION() void GoToNewGameOptions();

private:

	UPROPERTY() UGridPanel* MainMenuGrid;

	UPROPERTY() UNewGameOptionsWidget* NewGameOptionsWidget;
	UPROPERTY() UCancelButtonWidget* CancelButtonWidget;
};