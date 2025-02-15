// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrewDetailsGridWidget.generated.h"

class UImage;
class UButton;
class UCrewDetailsGridStatWidget; 

UCLASS()
class SV_API UCrewDetailsGridWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;

	void InitialiseGridForCrewMember(FGuid crewMember);

protected:

	UPROPERTY(meta = (BindWidget)) UButton* PrimaryBtn;

	UPROPERTY(meta = (BindWidget)) UUserWidget* EquipmentSlot1;
	UPROPERTY(meta = (BindWidget)) UUserWidget* EquipmentSlot2;
	UPROPERTY(meta = (BindWidget)) UUserWidget* EquipmentSlot3;

	UPROPERTY(meta = (BindWidget)) UCrewDetailsGridStatWidget* CrewStatWidget;

	UPROPERTY(meta = (BindWidget)) UImage* PrimaryImage;
	UPROPERTY(meta = (BindWidget)) UImage* SecondaryImage;

	UFUNCTION() void PrimaryClicked();
	UFUNCTION() void Tool1Clicked();
	UFUNCTION() void Tool2Clicked();
	UFUNCTION() void Tool3Clicked();

	void OpenToolSelectionMenu(int toolIndex);

private:
	void ResetDetailsGrid();

	void ResetPrimaryInventoryImage(UImage* image);
	void ResetEquipmentInventoryImage(UImage* image);


	UPROPERTY() FGuid CurrentCrewId;
};
