// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../../Interfaces/DamageReceiver.h"
#include "ClothingDamageReceiveComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(SVLogClothingDamageReceiveComponent, Log, All);

class UClothingStatsComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UClothingDamageReceiveComponent : public UActorComponent, public IDamageReceiver
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UClothingDamageReceiveComponent();

    virtual float DoDamage(float multiplier, int damage, float impulseDamage = 500.0f, FRotator angleOfDamage = FRotator::ZeroRotator, FVector locationOfDamage = FVector::ZeroVector) override;
    virtual float DamageToMitigateFromBullet(float multiplier, int damage) override;

protected:
    UClothingStatsComponent *GetStatsComponent();
};
