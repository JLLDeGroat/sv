// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/AnimAccessComponent.h"
#include "DamageRecieveComponent.generated.h"

class UBaseRunnable;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UDamageRecieveComponent : public UAnimAccessComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDamageRecieveComponent(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float DoDamage(float multiplier, int damage, float impulseDamage = 500.0f, FRotator angleOfDamage = FRotator::ZeroRotator, FVector locationOfDamage = FVector::ZeroVector);

protected:
	UFUNCTION() void OnDeathHandleCallback();

private:

	FTimerHandle DeathHandle;
	UPROPERTY() UBaseRunnable* WinLossRunnable;

};
