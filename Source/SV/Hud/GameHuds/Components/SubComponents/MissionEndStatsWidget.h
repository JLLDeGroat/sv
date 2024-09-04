// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionEndStatsWidget.generated.h"

class UVerticalBox;
class UMissionEndStatItemWidget;
UCLASS()
class SV_API UMissionEndStatsWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

protected:

	UFUNCTION() void OnShow();
	UPROPERTY(meta = (BindWidget)) UVerticalBox* StatBox;


private:

	void CreateStatItem(FString statName, FString statValue);

	UMissionEndStatItemWidget* CreateStatItemWidget();
};
