// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FOnGunFireInput.generated.h"

/**
 *
 */
USTRUCT()
struct FOnGunFireInput
{
    GENERATED_BODY()
public:
    FOnGunFireInput()
    {
        FirerActor = nullptr;
        Target = nullptr;
        StartLocation = FVector::ZeroVector;
        DesiredLocation = FVector::ZeroVector;
    }

    static FOnGunFireInput Create(AActor *firerActor, AActor *target, FVector startLoc, FVector desiredLoc, AActor *bullet)
    {
        auto input = FOnGunFireInput();
        input.FirerActor = firerActor;
        input.Target = target;
        input.StartLocation = startLoc;
        input.DesiredLocation = desiredLoc;
        input.Bullet = bullet;

        return input;
    }

    AActor *GetFirerActor() { return FirerActor; }
    AActor *GetTargetActor() { return Target; }
    AActor *GetBullet() { return Bullet; }
    FVector GetStartLocation() { return StartLocation; }
    FVector GetDesiredLocation() { return DesiredLocation; }

protected:
    UPROPERTY()
    AActor *FirerActor;
    UPROPERTY()
    AActor *Target;
    UPROPERTY()
    AActor *Bullet;
    UPROPERTY()
    FVector StartLocation;
    UPROPERTY()
    FVector DesiredLocation;
};
