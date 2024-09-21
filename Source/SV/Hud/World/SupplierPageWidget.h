// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../Interfaces/UI/WorldPage.h"
#include "SupplierPageWidget.generated.h"


class USupplyListWidget;

UCLASS()
class SV_API USupplierPageWidget : public UUserWidget, public IWorldPage
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	virtual void OpenPage() override;

protected:

	UPROPERTY(meta = (BindWidget)) USupplyListWidget* SupplyList;
};
