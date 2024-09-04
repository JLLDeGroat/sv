// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UInventoryComponent::AddToHeldResource(EResourceType rType, int amount) {
	if (HeldResource.Find(rType)) {
		HeldResource[rType] += amount;
	}
	else {
		HeldResource.Add(rType);
		HeldResource[rType] += amount;
	}
}

TMap<EResourceType, int> UInventoryComponent::GetHeldResources() {
	return HeldResource;
}

void UInventoryComponent::AddToIntel() {
	Intel += 1;
}
int UInventoryComponent::GetIntel() {
	return Intel;
}