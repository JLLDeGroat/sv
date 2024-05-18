// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterDetailsComponent.h"

// Sets default values for this component's properties
UCharacterDetailsComponent::UCharacterDetailsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCharacterDetailsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UCharacterDetailsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterDetailsComponent::SetCharacterControl(ECharacterControl characterControl) {
	ControlType = characterControl;
}
ECharacterControl UCharacterDetailsComponent::GetCharacterControl() const {
	return ControlType;
}


void UCharacterDetailsComponent::SetHealth(int health) {
	Health = health;
}
int UCharacterDetailsComponent::GetHealth() const {
	return Health;
}

void UCharacterDetailsComponent::RemoveHealth(int health, bool& isDead) {
	Health -= health;

	if (Health < 0) {
		Health = 0;
		isDead = true;
	}
}