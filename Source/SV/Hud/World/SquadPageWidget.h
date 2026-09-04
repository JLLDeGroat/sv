// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../Interfaces/UI/WorldPage.h"
#include "SquadPageWidget.generated.h"

class UCrewDADDetails;
class USquadListWidget;
class UCrewDetailsGridWidget;
class USelectEquipmentListWidget;

UCLASS()
class SV_API USquadPageWidget : public UUserWidget, public IWorldPage
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void OpenPage() override;

	UCrewDADDetails* GetCrewDADDetails();

protected:
	UPROPERTY(meta = (BindWidget))
	UCrewDADDetails *CrewDaDDetails;

	UPROPERTY(meta = (BindWidget))
	USquadListWidget *SquadListWidget;

	UPROPERTY(meta = (BindWidget))
	UCrewDetailsGridWidget *CrewDetailsGrid;

	UPROPERTY(meta = (BindWidget))
	USelectEquipmentListWidget *SelectPrimaryList;
};
