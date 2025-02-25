// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DestructibleDetails.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UDestructibleDetails : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UDestructibleDetails(const FObjectInitializer &ObjectInitializer);

    void SetHealth(float health);

    float GetHealth();
    float GetMaxHealth();
    void RemoveFromHealth(float health);
    bool GetIsDead();

protected:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float Health;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float MaxHealth;
};
