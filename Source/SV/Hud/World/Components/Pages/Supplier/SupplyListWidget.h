// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SupplyListWidget.generated.h"

class UScrollBox;
class USupplierListItemWidget;

UCLASS()
class SV_API USupplyListWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void FillSupplyList();

protected:

	UPROPERTY(meta = (BindWidget)) UScrollBox* SupplyScroll;

	USupplierListItemWidget* CreateSupplyListItemWidget();

};
