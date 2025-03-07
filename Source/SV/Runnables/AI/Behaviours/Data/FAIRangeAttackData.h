// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FAIRangeAttackData.generated.h"

USTRUCT()
struct FAIRangeAttackData
{
    GENERATED_BODY()
public:
    FAIRangeAttackData()
    {
    }

protected:
};

USTRUCT()
struct FWhereToShoot
{
    GENERATED_BODY()
public:
    FWhereToShoot()
    {
        bIsPreferredLocation = false;
        bPassesThroughDestructionMesh = false;
        Accuracy = 0;
        Location = FVector::ZeroVector;
        bIsValid = false;
    }

    bool GetIsPreferredLocation() const { return bIsPreferredLocation; }
    bool GetPassesThroughDestructionMesh() const { return bPassesThroughDestructionMesh; }

    void SetIsPreferredLocation(bool val) { bIsPreferredLocation = val; }
    void SetPassesThroughDestructionMesh(bool val) { bPassesThroughDestructionMesh = val; }

    float GetAccuracy() { return Accuracy; }
    void SetAccuracy(float acc) { Accuracy = acc; }

    FVector GetLocation() { return Location; }
    void SetLocation(FVector loc) { Location = loc; }

    void SetIsValid() { bIsValid = true; }
    void SetIsInvalid() { bIsValid = false; }
    bool GetIsValid() { return bIsValid; }

protected:
    UPROPERTY()
    bool bIsPreferredLocation;

    UPROPERTY()
    bool bPassesThroughDestructionMesh;

    UPROPERTY()
    bool bIsValid;

    UPROPERTY()
    float Accuracy;

    UPROPERTY()
    FVector Location;
};


