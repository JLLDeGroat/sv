// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ClothingStatsComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UClothingStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UClothingStatsComponent();

	void SetHealth(int newHealth);
	int GetHealth();
	void SetMaxHealth(int newMaxHealth);
	int GetMaxHealth();
	void RemoveFromHealth(int health);
	void AddToHealth(int health);

	void SetDamageMitigation(float mitigation);
	float GetDamageMitigation();

protected:
	UPROPERTY()
	int Health;
	UPROPERTY()
	int MaxHealth;
	UPROPERTY()
	float DamageMitigation;
};
