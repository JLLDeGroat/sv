// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../../../../Enums/EEquipmentEnums.h"
#include "SelectEquipmentListWidget.generated.h"

class USelectEquipmentListItemWidget;
class UScrollBox;

/**
 *
 */
UCLASS()
class SV_API USelectEquipmentListWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	void OpenListForTools(FGuid memberId, int toolIndex);

	void OpenListForPrimaries(FGuid memberId);
	void PrimaryButtonClicked(EGun gunType, FGuid primaryId);
	void ToolButtonClicked(EToolType toolType, uint8 tool);

	protected:

	UPROPERTY(meta = (BindWidget)) UScrollBox* ScrollBox; 

private:

	USelectEquipmentListItemWidget* CreateListItem();

	void ResetDetailsGrid();

	UPROPERTY() FGuid CurrentMemberId;
	UPROPERTY() int CurrentToolIndex;
};
