// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/AnimAccessComponent.h"
#include "../../Interfaces/DamageReceiver.h"
#include "DamageReceiveComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDamageReceiveComponent, Log, All);

class UBaseRunnable;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UDamageReceiveComponent : public UAnimAccessComponent, public IDamageReceiver
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDamageReceiveComponent(const FObjectInitializer &ObjectInitializer);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	virtual float DoDamage(float multiplier, int damage, float impulseDamage = 500.0f, FRotator angleOfDamage = FRotator::ZeroRotator, FVector locationOfDamage = FVector::ZeroVector) override;
	virtual float DamageToMitigateFromBullet(float multiplier, int damage) override;

protected:
	UFUNCTION()
	void OnDeathHandleCallback();
	UFUNCTION()
	void OnDeathPhysicsHandleCallback();

private:
	FTimerHandle DeathHandle;
	UPROPERTY()
	UBaseRunnable *WinLossRunnable;

	FTimerHandle DeathPhysicsHandle;
};
