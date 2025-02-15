// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CDGSItemWidget.generated.h"

class UTextBlock;
/**
 *
 */
UCLASS()
class SV_API UCDGSItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetTitle(FString title);
	void SetValue(int value);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock *StatTitle;
	UPROPERTY(meta = (BindWidget))
	UTextBlock *StatValue;
};
