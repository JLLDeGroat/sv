// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BloodHitEffect.generated.h"

class UNiagaraComponent;
class UGeometryCacheComponent;

UCLASS()
class SV_API ABloodHitEffect : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABloodHitEffect();

	void MoveBackSplatter(float movement);

	void SetBloodSpatterAssetFor(AActor* actor);


protected:

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UNiagaraComponent* BloodSpatter;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UGeometryCacheComponent* BackBloodSplatter;

	UFUNCTION() void OnDelay();
	UFUNCTION() void OnBackSpatterComplete();
	FTimerHandle OnAutoKill;
	FTimerHandle OnBackSpatterCompleteAnim;

private:

	UPROPERTY() float SpatterSpeed = 4.0f;

};
