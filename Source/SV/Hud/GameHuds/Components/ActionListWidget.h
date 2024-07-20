// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../../Enums/ECharacterEnums.h"
#include "ActionListWidget.generated.h"

class UActionListItemWidget;
/**
 *
 */
UCLASS()
class SV_API UActionListWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

protected:

	UFUNCTION() void CreateAndAddActionItem(EActionType actionType, FString ShortCut);
	UFUNCTION() void ClearVerticalBox();
	UFUNCTION() void HideOrReset();

private:

	UActionListItemWidget* CreateListItem();

};
