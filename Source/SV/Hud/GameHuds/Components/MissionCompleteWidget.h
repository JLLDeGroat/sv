// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionCompleteWidget.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UMissionCompleteWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

protected:

	UFUNCTION() void OnConsumeDelegate();

	UFUNCTION() void OnContinueClicked();
};
