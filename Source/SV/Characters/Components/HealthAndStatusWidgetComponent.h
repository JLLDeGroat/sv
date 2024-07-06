// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthAndStatusWidgetComponent.generated.h"

class UCameraComponent;
class UProgressBar;

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


	void SetName(FString name) const;
	void SetPercentage(float value) const;

	void UpdateOnHealthChange();

protected:

	UFUNCTION() void InternalHealthChangeCallback();
	UFUNCTION() void RecentHealthChangeCallback();

private:

	UPROPERTY() UCameraComponent* CameraComponent;

	UPROPERTY() FTimerHandle InternalHealthChangeTimer;
	UPROPERTY() FTimerHandle RecentHealthChangeTimer;


	UPROPERTY() float CurrentPercent;
	UPROPERTY() float RecentPercentChange;

	float GetCurrentPercentage();

	UPROPERTY() FString PercentName = "HealthBar";
	UPROPERTY() FString DynamicPercentName = "DynamicHealthBar";
	UPROPERTY() FString NameTag = "NameTag";

	UPROPERTY() int CurrentHealth;
	UPROPERTY() int RecentHealthChange;
	UPROPERTY() int MaxHealth;

	UPROPERTY() float HealthChangeDelay;

	UPROPERTY() UProgressBar* HealthProgressBar;
	UPROPERTY() UProgressBar* DynamicHealthProgressBar;

	float GetPercentageOfHealth(float value, float maxValue);

};
