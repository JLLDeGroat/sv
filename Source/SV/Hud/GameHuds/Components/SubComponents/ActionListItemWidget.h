// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../../../Enums/ECharacterEnums.h"
#include "ActionListItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UActionListItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void SetupActionItem(EActionType actionType, FString shortCut);

protected:

	UFUNCTION() void OnButtonClick();
	UFUNCTION() void OnButtonUnHovered();
	UFUNCTION() void OnButtonHovered();

private:

	UPROPERTY() EActionType Action;
	UPROPERTY() FString ShortCut;
};
