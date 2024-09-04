// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupDetailsComponent.h"

// Sets default values for this component's properties
UPickupDetailsComponent::UPickupDetailsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UPickupDetailsComponent::SetResourceType(EResourceType rType) {
	ResourceType = rType;
}
void UPickupDetailsComponent::SetAmount(int amount) {
	Amount = amount;
}

int UPickupDetailsComponent::GetAmount() {
	return Amount;
}
EResourceType UPickupDetailsComponent::GetResourceType() {
	return ResourceType;
}
void UPickupDetailsComponent::SetIsIntel() {
	bIsIntel = true;
}
bool UPickupDetailsComponent::GetIsIntel() {
	return bIsIntel;
}