// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CrewMemberGen.generated.h"

class UBaseDD;
/**
 *
 */
UCLASS()
class SV_API UCrewMemberGen : public UObject
{
    GENERATED_BODY()

public:
    UCrewMemberGen(const FObjectInitializer &ObjectInitializer);

    void GenerateCrewMembers(int amount);

    TArray<TSubclassOf<UBaseDD>> GetStartingBaseDDClass(int amount = 5);

protected:
    UPROPERTY()
    FRandomStream RandomStream;
};
