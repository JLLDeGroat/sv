// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BloodHitEffect.generated.h"

class UNiagaraComponent;

UCLASS()
class SV_API ABloodHitEffect : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABloodHitEffect();


protected:

	virtual void BeginPlay() override;

	UPROPERTY() UNiagaraComponent* BloodSpatter;

	UFUNCTION() void OnDelay();
	FTimerHandle OnAutoKill;

};
