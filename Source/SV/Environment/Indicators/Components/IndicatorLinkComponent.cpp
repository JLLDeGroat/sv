// Fill out your copyright notice in the Description page of Project Settings.


#include "IndicatorLinkComponent.h"
#include "../../Pickups/Components/PickupIndicatorLink.h"

// Sets default values for this component's properties
UIndicatorLinkComponent::UIndicatorLinkComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PickupIndicatingTo = nullptr;
}


void UIndicatorLinkComponent::SetPickupIndicatingTo(AActor* actor) {
	PickupIndicatingTo = actor;
	auto pickupIndicator = actor->GetComponentByClass<UPickupIndicatorLink>();
	if (pickupIndicator)
		pickupIndicator->SetIndicatorIndicatingTo(GetOwner());
}
AActor* UIndicatorLinkComponent::GetPickupIndicatingTo() {
	return PickupIndicatingTo;
}
