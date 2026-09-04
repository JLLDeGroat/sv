// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrewDetailsGridStatWidget.generated.h"

class UCDGSItemWidget;
class UCDGDDItemWidget;

class UTextBlock;
class UVerticalBox;
class UScrollBox;
class UBaseDD;
class UButton;
class UCrewDetailsGridWidget;
/**
 *
 */
UCLASS()
class SV_API UCrewDetailsGridStatWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    void SetDetailsForCrew(TArray<TSubclassOf<UBaseDD>> dAndDs, TMap<FString, int> stats);

    void SetCrewDetailsWidget(UCrewDetailsGridWidget *detailsGridWidget);

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock *StatTitle;

    UPROPERTY(meta = (BindWidget))
    UTextBlock *DDTitle;

    UPROPERTY(meta = (BindWidget))
    UVerticalBox *StatBox;
    UPROPERTY(meta = (BindWidget))
    UVerticalBox *DDBox;

    UCDGSItemWidget *CreateStatItem();
    UCDGDDItemWidget *CreateDDItem();

    UPROPERTY(meta = (BindWidget))
    UButton *TraitButton;

    UPROPERTY()
    TArray<TSubclassOf<UBaseDD>> DAndDs;

    UPROPERTY() UCrewDetailsGridWidget *DetailsGrid;

    void ClearStatGrid();

    UFUNCTION()
    void OnTraitClicked();
};
