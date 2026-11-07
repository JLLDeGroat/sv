// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthAndStatusWidgetComponent.generated.h"

class UCameraComponent;
class UProgressBar;
class UHealthAndStatusWidget;

SV_API DECLARE_LOG_CATEGORY_EXTERN(SVLogHealthAndStatusWidgetComponent, Log, All);
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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void SetName(FString name);
	void SetPercentage(float value);

	void UpdateOnHealthChange();
	float UpdateProgressBarOnHealthChange(float healthValue, float maxHealthValue, UProgressBar *progressBar, UProgressBar *dynamicProgressBar);

	void TakenDamage(int amount);
	void TakenStatusEffect(FString status);

	void UpdateStatusWidgetData();

	void HideComponent();
	void ShowComponent();

protected:
	UFUNCTION()
	void InternalHealthChangeCallback();
	UFUNCTION()
	void RecentHealthChangeCallback();

	UFUNCTION()
	void OnDamageResetTimerCallback();
	UFUNCTION()
	void OnStatusEffectTimerCallback();

private:
	UPROPERTY()
	UCameraComponent *CameraComponent;

	UPROPERTY()
	FTimerHandle InternalHealthChangeTimer;
	UPROPERTY()
	FTimerHandle RecentHealthChangeTimer;

	UPROPERTY()
	FTimerHandle DamageTakenResetTimer;
	UPROPERTY()
	FTimerHandle StatusEffectResetTimer;

	UPROPERTY()
	int TotalTakenDamage;

	UPROPERTY()
	float CurrentPercent;
	UPROPERTY()
	float RecentPercentChange;

	UPROPERTY()
	int CurrentHealth;
	UPROPERTY()
	int RecentHealthChange;
	UPROPERTY()
	int MaxHealth;

	UPROPERTY()
	int ArmArmour;
	UPROPERTY()
	int RecentArmArmourChange;
	UPROPERTY()
	int MaxArmArmour;

	UPROPERTY()
	int ShoulderArmour;
	UPROPERTY()
	int RecentShoulderArmourChange;
	UPROPERTY()
	int MaxShoulderArmour;

	UPROPERTY()
	int LegArmour;
	UPROPERTY()
	int RecentLegArmourChange;
	UPROPERTY()
	int MaxLegArmour;

	UPROPERTY()
	int ChestArmour;
	UPROPERTY()
	int RecentChestArmourChange;
	UPROPERTY()
	int MaxChestArmour;

	UPROPERTY()
	int HelmetArmour;
	UPROPERTY()
	int RecentHelmetArmourChange;
	UPROPERTY()
	int MaxHelmetArmour;

	UPROPERTY()
	bool bUseHelmetArmourBar;
	UPROPERTY()
	bool bUseLegArmourBar;
	UPROPERTY()
	bool bUseShoulderArmourBar;
	UPROPERTY()
	bool bUseChestArmourBar;
	UPROPERTY()
	bool bUseArmArmourBar;

	UPROPERTY()
	float HealthChangeDelay;

	float GetPercentageOfHealth(float value, float maxValue);

	FORCEINLINE UHealthAndStatusWidget *GetHealthWidget();

	FORCEINLINE float SetZeroIfBelow(float value);

	int DetermineArmourWidgetGridLocations(bool bIsValidBar, UProgressBar* progressBar, UProgressBar* dynamicBar, int nextValidGridLocation);
};
