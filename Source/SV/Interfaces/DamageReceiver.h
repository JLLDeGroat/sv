// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageReceiver.generated.h"

/**
 *
 */
UINTERFACE()
class SV_API UDamageReceiver : public UInterface
{
    GENERATED_BODY()
};

class SV_API IDamageReceiver
{

    GENERATED_BODY()

public:
    virtual float DoDamage(float multiplier, int damage, float impulseDamage = 500.0f, FRotator angleOfDamage = FRotator::ZeroRotator, FVector locationOfDamage = FVector::ZeroVector) = 0;
    virtual float DamageToMitigateFromBullet(float multiplier, int damage) = 0;
};