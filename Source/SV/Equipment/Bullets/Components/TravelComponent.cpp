// Fill out your copyright notice in the Description page of Project Settings.


#include "TravelComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UTravelComponent::UTravelComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTravelComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
}


// Called every frame
void UTravelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto newLocation = UKismetMathLibrary::VInterpTo_Constant(GetOwner()->GetActorLocation(), TravelToLocation, DeltaTime, 100);
	GetOwner()->SetActorLocation(newLocation);

	if (FVector::Dist(TravelToLocation, GetOwner()->GetActorLocation()) < 10) {
		TravelToLocation = GetOwner()->GetActorLocation() + (5000 * GetOwner()->GetActorForwardVector());
	}
}

void UTravelComponent::StartTravel(FVector location) {
	TravelToLocation = location;

	SetComponentTickEnabled(true);
}

