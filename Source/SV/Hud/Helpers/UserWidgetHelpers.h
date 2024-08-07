// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../Enums/EEquipmentEnums.h"
#include "../../Enums/ECharacterEnums.h"
#include "UserWidgetHelpers.generated.h"

class UImage;
class UUserWidget;
class UTexture2D;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class SV_API UUserWidgetHelpers : public UObject
{
	GENERATED_BODY()

public:
	
	static UImage* GetImageFromWidget(UUserWidget* widget, FString widgetName);
	static UButton* GetButtonFromWidget(UUserWidget* widget, FString widgetName);
	static UTextBlock* GetTextBlockFromWidget(UUserWidget* widget, FString widgetName);

	static UTexture2D* GetTextureForGun(EGun gunType);
	static UTexture2D* GetTextureForTool(EToolType toolType, uint8 tool);

	static UTexture2D* GetPrimaryNonTexture();
	static UTexture2D* GetEquipmentNonTexture();

	static UTexture2D* GetTextureForActionType(EActionType actionType);
};
