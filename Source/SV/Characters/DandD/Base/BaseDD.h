// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../../Enums/EConfigurationEnums.h"
#include "BaseDD.generated.h"

/**
 *
 */
UCLASS()
class SV_API UBaseDD : public UObject
{
    GENERATED_BODY()

public:
    UBaseDD(const FObjectInitializer &ObjectInitializer);
    FString GetDeviationOrDirectiveDescription();
    FString GetDeviationOrDirectiveTitle();
    EDDType GetDeviationOrDirective();

    bool GetActivatedThisLevel();
    bool GetActivatedThisTurn();

    void ResetOnNewTurn();

protected:
    UPROPERTY()
    FString Title;
    UPROPERTY()
    FString Description;

    UPROPERTY()
    EDDType DeviationOrDirective;

    UPROPERTY()
    bool bActivatedThisTurn;
    UPROPERTY()
    bool bActivatedThisLevel;

    UPROPERTY()
    bool bCanStartOutWith;
};
