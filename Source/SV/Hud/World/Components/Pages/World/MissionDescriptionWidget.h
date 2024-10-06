// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionDescriptionWidget.generated.h"

class UTextBlock;
class UButton;
class UImage;

UCLASS()
class SV_API UMissionDescriptionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;


	bool GetMoveCompleted() const;

protected:

	UPROPERTY(meta = (BindWidget)) UTextBlock* MissionTitleText;
	UPROPERTY(meta = (BindWidget)) UTextBlock* MissionTypeText;
	UPROPERTY(meta = (BindWidget)) UTextBlock* MissionDescriptionText;
	UPROPERTY(meta = (BindWidget)) UTextBlock* MissionFluffText;

	UPROPERTY(meta = (BindWidget)) UButton* StartMissionBtn;

	UFUNCTION() void OnWorldMoveComplete(FVector2D MovedToLocation);
	UFUNCTION() void OnStartMissionClicked();


	UPROPERTY() FString MissionName;
	UPROPERTY() uint8 MissionType;

	UPROPERTY(meta = (BindWidget)) UImage* MissionImage;

private:

	UPROPERTY() bool MoveCompleted;
};
