// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrownOwnerComponent.h"

// Sets default values for this component's properties
UThrownOwnerComponent::UThrownOwnerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UThrownOwnerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UThrownOwnerComponent::SetThrownOwner(AActor* actor) {
	ThrownOwner = actor;
}
AActor* UThrownOwnerComponent::GetThrownOwner() {
	return ThrownOwner;
}