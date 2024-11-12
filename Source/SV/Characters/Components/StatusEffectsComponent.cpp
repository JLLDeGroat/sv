// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffectsComponent.h"

// Sets default values for this component's properties
UStatusEffectsComponent::UStatusEffectsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UStatusEffectsComponent::AddToDebuff(EDebuffType debuffType, float amount) {
	if (DebuffMap.Find(debuffType))
		DebuffMap[debuffType] += amount;
	else
		DebuffMap.Add(debuffType, amount);
}
void UStatusEffectsComponent::AddToDebuff(TMap<EDebuffType, float> debuffs) {
	for (const TPair<EDebuffType, float>& pair : debuffs) {
		AddToDebuff(pair.Key, pair.Value);
	}
}
bool UStatusEffectsComponent::TryGetDebuffValue(EDebuffType debuffType, float& amount) {
	amount = 0.0f;

	if (DebuffMap.Find(debuffType)) {
		amount = DebuffMap[debuffType];
		return true;
	}

	return false;
}