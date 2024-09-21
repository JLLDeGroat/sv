// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../../Enums/EEquipmentEnums.h"
#include "CurrentResourceWidget.generated.h"

class UVerticalBox;
class UCurrentResourceItemWidget;
/**
 *
 */
UCLASS()
class SV_API UCurrentResourceWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

protected:

	UPROPERTY(meta = (BindWidget)) UVerticalBox* ResourceBox;

	UFUNCTION() void OnResourceChange(EResourceType rType);

private:

	void AddResourceItem(EResourceType rType, int amount);

	UCurrentResourceItemWidget* CreateResourceItemWidget();

	TArray<UCurrentResourceItemWidget*> ResourceItems;

};
