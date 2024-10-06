// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../../../../Enums/EEquipmentEnums.h"
#include "SelectEquipmentListItemWidget.generated.h"

class UButton;
class UTextBlock;

/**
 *
 */
UCLASS()
class SV_API USelectEquipmentListItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	EGun GetPrimaryGun();
	void SetPrimaryGun(EGun primary);

	FGuid GetItemId();
	void SetItemId(FGuid id);

	void SetToolAndType(EToolType toolType, uint8 tool);
	EToolType GetToolType();
	uint8 GetTool();

	int AddToAmount(int amount);

	void AddPrimaryDelegate();
	void AddToolDelegate();

protected:

	UFUNCTION() void ButtonClickedPrimary();
	UFUNCTION() void ButtonClickedTool();

	UPROPERTY() EGun PrimaryGun;
	UPROPERTY() int Amount;

	UPROPERTY() FGuid ItemId;

	UPROPERTY() EToolType ToolType;
	UPROPERTY() uint8 Tool;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UTextBlock* AmountText;

private:
	UButton* GetActivateButton();

};
