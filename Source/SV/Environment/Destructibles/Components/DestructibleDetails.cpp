// Fill out your copyright notice in the Description page of Project Settings.

#include "DestructibleDetails.h"

UDestructibleDetails::UDestructibleDetails(const FObjectInitializer &ObjectInitializer)
    : UActorComponent(ObjectInitializer)
{
}

void UDestructibleDetails::SetHealth(float health)
{
    Health = health;
    MaxHealth = health;
}

float UDestructibleDetails::GetHealth()
{
    return Health;
}
float UDestructibleDetails::GetMaxHealth()
{
    return MaxHealth;
}
void UDestructibleDetails::RemoveFromHealth(float health)
{
    Health -= health;
}
bool UDestructibleDetails::GetIsDead()
{
    return Health <= 0;
}