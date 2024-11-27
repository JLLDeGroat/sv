// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvironmentDetailsComponent.h"

// Sets default values for this component's properties
UEnvironmentDetailsComponent::UEnvironmentDetailsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Thickness = 5;
	AffectsFog = false;
	// ...
}

void UEnvironmentDetailsComponent::SetThickness(int value) {
	Thickness = value;
}
int UEnvironmentDetailsComponent::GetThickness() {
	return Thickness;
}

void UEnvironmentDetailsComponent::SetAffectsFog(bool affectsFog) {
	AffectsFog = affectsFog;
}
bool UEnvironmentDetailsComponent::GetAffectsFog() {
	return AffectsFog;
}