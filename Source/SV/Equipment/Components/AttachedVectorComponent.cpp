// Fill out your copyright notice in the Description page of Project Settings.


#include "AttachedVectorComponent.h"

// Sets default values for this component's properties
UAttachedVectorComponent::UAttachedVectorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UAttachedVectorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAttachedVectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UAttachedVectorComponent::GetAttachmentsForType(EAttachType attachType, FVector& location, FRotator& rotation) {
	if (attachType == EAttachType::AT_RightHand) {
		location = AttachedLocation;
		rotation = AttachedRotation;
		return true;
	}
	else if (attachType == EAttachType::AT_Backpack) {
		location = HolsteredLocation;
		rotation = HolsteredRotation;
		return true;
	}

	return false;
}

void UAttachedVectorComponent::SetAttachedVectors(FVector location, FRotator rotation) {
	AttachedLocation = location;
	AttachedRotation = rotation;
}
void UAttachedVectorComponent::SetHolsteredVectors(FVector location, FRotator rotation) {
	HolsteredLocation = location;
	HolsteredRotation = rotation;
}

FVector UAttachedVectorComponent::GetAttachedLocation() {
	return HolsteredLocation;
}
FRotator UAttachedVectorComponent::GetAttachedRotation() {
	return AttachedRotation;
}

FVector UAttachedVectorComponent::GetHolsteredLocation() {
	return HolsteredLocation;
}
FRotator UAttachedVectorComponent::GetHolsteredRotation() {
	return HolsteredRotation;
}