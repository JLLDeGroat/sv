// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArmourHitEffect.generated.h"

class UNiagaraComponent;

UCLASS()
class SV_API AArmourHitEffect : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AArmourHitEffect();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UNiagaraComponent *MetalNiagaraComponent;

    UPROPERTY()
    FTimerHandle OnAutoKill;

    UFUNCTION()
    void OnAutoKillCallback();

private:
};
