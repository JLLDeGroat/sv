// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../../../Enums/EEquipmentEnums.h"
#include "CurrentResourceItemWidget.generated.h"

class UImage;
class UTextBlock;

/**
 *
 */
UCLASS()
class SV_API UCurrentResourceItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	void SetResourceImage(UTexture2D* texture);
	void SetResourceText(FString text);


	EResourceType GetResourceType();
	void SetResourceType(EResourceType rType);

protected:

	UPROPERTY(meta = (BindWidget)) UImage* ResourceImage;
	UPROPERTY(meta = (BindWidget)) UTextBlock* ResourceText;

private:

	UPROPERTY() EResourceType ResourceType;

};
