// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TargetDetailsRowItemWidget.generated.h"

class UButton;
class UImage;
/**
 *
 */
UCLASS()
class SV_API UTargetDetailsRowItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	void SetSourceLocation(FVector sourceLocation);
	void SetTargetLocation(FVector targetLocation);
	void SetId(FGuid id);

	FVector GetSourceLocation() const;
	FVector GetTargetLocation() const;
	FGuid GetId() const;

	UButton* GetItemButton() const;

	void SetImage(UTexture2D* Texture);
protected:

	UFUNCTION() void OnButtonClicked();

	UFUNCTION() void OnButtonUnHovered();
	UFUNCTION() void OnButtonHovered();

	UPROPERTY(meta = (BindWidget)) UImage* RowItemImage;

private:

	UPROPERTY() FVector SourceLocation;
	UPROPERTY() FVector TargetLocation;
	UPROPERTY() FGuid Id;
};
