// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConstructSuicideEffect.generated.h"

class UNiagaraComponent;
class USphereComponent;

UCLASS()
class SV_API AConstructSuicideEffect : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AConstructSuicideEffect();

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UNiagaraComponent* ExplosionComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) USphereComponent* CollisionComponent;


	UFUNCTION() void Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY() TArray<AActor*> ActorsCaughtInExplosion;
};
