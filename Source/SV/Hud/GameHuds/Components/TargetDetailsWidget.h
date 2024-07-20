// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TargetDetailsWidget.generated.h"

class UVerticalBox;
class UHorizontalBox;
class UTargetDetailsRowWidget;
class UTargetDetailsRowItemWidget;

/**
 * 
 */
UCLASS()
class SV_API UTargetDetailsWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

protected:

	UFUNCTION() void OnAddTargetData(FGuid Id, FVector SourceLocation, FVector TargetLocation);
	UFUNCTION() void OnClearTargetData();
	UFUNCTION() void OnTargetIconClicked(FGuid Id, FVector Location);
	UFUNCTION() void HideOrReset();

	UFUNCTION() void CycleTarget();

private:

	UVerticalBox* GetDetailsBox() const;
	UHorizontalBox* GetLatestHorizontalBox(UVerticalBox* box);
	UHorizontalBox* GetHorizontalBoxFromWidget(UUserWidget* targetDetailsRowWidget);

	TArray<UTargetDetailsRowItemWidget*> GetItemsFromHorizontalBox(UHorizontalBox* box);

	UTargetDetailsRowWidget* CreateTargetDetailsRowWidget() const;
	UTargetDetailsRowItemWidget* CreateTargetDetailsRowItemWidget() const;
	
};
