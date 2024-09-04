// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupIndicatorLink.h"

// Sets default values for this component's properties
UPickupIndicatorLink::UPickupIndicatorLink()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UPickupIndicatorLink::SetIndicatorIndicatingTo(AActor* actor) {
	IndicatorIndicatingTo = actor;
}
AActor* UPickupIndicatorLink::GetIndicatorIndicatingTo() {
	return IndicatorIndicatingTo;
}