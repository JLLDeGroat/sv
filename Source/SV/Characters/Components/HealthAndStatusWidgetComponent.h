// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthAndStatusWidgetComponent.generated.h"

class UCameraComponent;
class UProgressBar;
class UHealthAndStatusWidget;
/**
 *
 */
UCLASS()
class SV_API UHealthAndStatusWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:

	UHealthAndStatusWidgetComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	void SetName(FString name);
	void SetPercentage(float value);

	void UpdateOnHealthChange();

	void TakenDamage(int amount);
	void TakenStatusEffect(FString status);

protected:

	UFUNCTION() void InternalHealthChangeCallback();
	UFUNCTION() void RecentHealthChangeCallback();

	UFUNCTION() void OnDamageResetTimerCallback();
	UFUNCTION() void OnStatusEffectTimerCallback();

private:

	UPROPERTY() UCameraComponent* CameraComponent;

	UPROPERTY() FTimerHandle InternalHealthChangeTimer;
	UPROPERTY() FTimerHandle RecentHealthChangeTimer;

	UPROPERTY() FTimerHandle DamageTakenResetTimer;
	UPROPERTY() FTimerHandle StatusEffectResetTimer;

	UPROPERTY() int TotalTakenDamage;

	UPROPERTY() float CurrentPercent;
	UPROPERTY() float RecentPercentChange;

	UPROPERTY() int CurrentHealth;
	UPROPERTY() int RecentHealthChange;
	UPROPERTY() int MaxHealth;

	UPROPERTY() float HealthChangeDelay;

	float GetPercentageOfHealth(float value, float maxValue);

	FORCEINLINE UHealthAndStatusWidget* GetHealthWidget();
};
