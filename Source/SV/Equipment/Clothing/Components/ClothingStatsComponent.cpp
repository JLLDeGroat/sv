// Fill out your copyright notice in the Description page of Project Settings.

#include "ClothingStatsComponent.h"

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
	Health -= health;
}
void UClothingStatsComponent::AddToHealth(int health)
{
	Health += health;
}