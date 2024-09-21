// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SupplierListCostItemWidget.generated.h"

class UTextBlock;
class UImage;
/**
 *
 */
UCLASS()
class SV_API USupplierListCostItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	void SetResourceImage(UTexture2D* texture);
	void SetResourceText(FString text);

protected:

	UPROPERTY(meta = (BindWidget)) UImage* ResourceImage;
	UPROPERTY(meta = (BindWidget)) UTextBlock* ResourceText;

};
