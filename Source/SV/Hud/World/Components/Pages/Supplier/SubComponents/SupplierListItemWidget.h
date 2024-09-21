// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../../../../../Enums/EEquipmentEnums.h"
#include "../../../../../../Data/FSupplyStockData.h"
#include "SupplierListItemWidget.generated.h"

class UImage;
class UTextBlock;
class USupplierListCostItemWidget;
class UGridPanel;
class UButton;

UCLASS()
class SV_API USupplierListItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	void SetNameOfItem(FString name);
	void SetBigImage(UTexture2D* texture);
	void SetDescriptionOfItem(FString desc);
	void SetAmountOwnedText(int amount);

	void AddCosts(TMap<EResourceType, int> costs);

	void ShortenImage();

	void SetStockItem(FSupplyStockItem* stockItem);

protected:
	UPROPERTY(meta = (BindWidget)) UImage* Image;
	UPROPERTY(meta = (BindWidget)) UTextBlock* ItemName;
	UPROPERTY(meta = (BindWidget)) UTextBlock* DescriptionText;
	UPROPERTY(meta = (BindWidget)) UTextBlock* AmountOwnedText;

	UPROPERTY(meta = (BindWidget)) UGridPanel* CostGrid;
	UPROPERTY(meta = (BindWidget)) UButton* PurchaseButton;

	UPROPERTY() TMap<EResourceType, int> Costs;
	UPROPERTY() int AmountOwned;

	UFUNCTION() void OnPurchaseClicked();

private:

	USupplierListCostItemWidget* CreateCostItemWidget();

	int GetColumnBasedOnResourceType(EResourceType resource);

	UPROPERTY() FSupplyStockItem StockItem;

	bool GetCanAfford();
};
