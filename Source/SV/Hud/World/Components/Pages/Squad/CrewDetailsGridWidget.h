// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrewDetailsGridWidget.generated.h"

class UImage;
/**
 *
 */
UCLASS()
class SV_API UCrewDetailsGridWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;

	void InitialiseGridForCrewMember(FGuid crewMember);

protected:

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
