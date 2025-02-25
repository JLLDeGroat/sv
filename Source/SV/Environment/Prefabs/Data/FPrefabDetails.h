// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FPrefabDetails.generated.h"

/**
 *
 */
USTRUCT()
struct FPrefabDetails
{
    GENERATED_BODY()
public:
    FPrefabDetails()
    {
    }

    FPrefabDetails(FString loc, int x, int y)
    {
        XLength = x;
        YLength = y;
        BlueprintLocation = loc;
    }

    int GetX() { return XLength; }
    int GetY() { return YLength; }  
    FString GetBlueprintLocation() { return BlueprintLocation; }

protected:
    UPROPERTY()
    int XLength;
    UPROPERTY()
    int YLength;

    UPROPERTY()
    FString BlueprintLocation;
};
