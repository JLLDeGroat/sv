// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionEndStatItemWidget.generated.h"

class UTextBlock;

UCLASS()
class SV_API UMissionEndStatItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	void SetNameAndValue(FString name, FString value);

protected:

	UPROPERTY(meta = (BindWidget)) UTextBlock* StatName;
	UPROPERTY(meta = (BindWidget)) UTextBlock* StatValue;

};
