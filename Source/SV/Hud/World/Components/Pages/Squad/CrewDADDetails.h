// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrewDADDetails.generated.h"

class UButton;
class UCDGDDItemWidget;
class USquadPageWidget;
class UBorder;
class UBaseDD;
class UVerticalBox;

UCLASS()
class SV_API UCrewDADDetails : public UUserWidget
{
    GENERATED_BODY()
public:
    virtual void NativeConstruct() override;

    void SetupDADDetails(TArray<TSubclassOf<UBaseDD>> danDs);
    void SetSquadPage(USquadPageWidget *page);

protected:
    UPROPERTY(meta = (BindWidget))
    UButton *CloseButton;

    UFUNCTION()
    void OnCloseClicked();

    UPROPERTY()
    USquadPageWidget *SquadPage;

    UPROPERTY()
    TArray<TSubclassOf<UBaseDD>> DAndDs;

    UPROPERTY(meta = (BindWidget))
    UBorder *BackBorder;

    UPROPERTY(meta = (BindWidget))
    UVerticalBox *DirectiveBox;
    UPROPERTY(meta = (BindWidget))
    UVerticalBox *DeviationBox;

private:
    UCDGDDItemWidget *CreateDDItem();
};
