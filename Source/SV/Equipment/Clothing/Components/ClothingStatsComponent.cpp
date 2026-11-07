// Fill out your copyright notice in the Description page of Project Settings.

#include "ClothingStatsComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

// Sets default values for this component's properties
UClothingStatsComponent::UClothingStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UClothingStatsComponent::SetHealth(int newHealth)
{
	Health = newHealth;
}
int UClothingStatsComponent::GetHealth()
{
	return Health;
}

void UClothingStatsComponent::SetMaxHealth(int newMaxHealth)
{
	MaxHealth = newMaxHealth;
}
int UClothingStatsComponent::GetMaxHealth()
{
	return MaxHealth;
}

void UClothingStatsComponent::SetDamageMitigation(float mitigation)
{
	DamageMitigation = mitigation;
}
float UClothingStatsComponent::GetDamageMitigation()
{
	return DamageMitigation;
}

void UClothingStatsComponent::RemoveFromHealth(int health)
{
	UDebugMessages::LogDisplay(this, "removing " + FString::SanitizeFloat(health, 2) + " from health " + FString::SanitizeFloat(Health, 2));
	Health -= health;
	UDebugMessages::LogDisplay(this, "health is now " + FString::SanitizeFloat(Health, 2));
}
void UClothingStatsComponent::AddToHealth(int health)
{
	Health += health;
}