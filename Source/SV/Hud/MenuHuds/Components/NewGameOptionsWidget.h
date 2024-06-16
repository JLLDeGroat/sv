// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../../Data/FMapGridData.h"
#include "../../../Enums/EConfigurationEnums.h"
#include "NewGameOptionsWidget.generated.h"

class UButton;
class UTextBlock;
class UBaseRunnable;

class UClassicGameModeWidget;

/**
 * 
 */
UCLASS()
class SV_API UNewGameOptionsWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;

	void ResetWidget();

	void OnGenCompleted();

protected:

	UFUNCTION() void OnClassicGameClicked();
	UFUNCTION() void OnStartGameClicked();

private:

	void SetStartButtonVisibility(bool val);
	void UpdateTitleAndDescriptions(EGameModeType gameMode);

	UButton* GetBeginGameButton();
	UButton* GetClassicGameButton();
	UTextBlock* GetTitleBlock();
	UTextBlock* GetDescriptionBlock();

	UPROPERTY() UClassicGameModeWidget* ClassicGameModeWidget;
	UPROPERTY() UBaseRunnable* GenThread;
	UPROPERTY() FMapGridData CurrentMapGrid;
};

