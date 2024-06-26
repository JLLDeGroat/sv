// Fill out your copyright notice in the Description page of Project Settings.


#include "ActivateTogglesComponent.h"
#include "../../Environment/Components/ToggleMovementComponent.h"
// Sets default values for this component's properties
UActivateTogglesComponent::UActivateTogglesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UActivateTogglesComponent::AddToggleComponent(UToggleMovementComponent* toggleComponent) {
	ToggleComponents.Emplace(toggleComponent);
}
void UActivateTogglesComponent::RemoveToggleComponent(UToggleMovementComponent* toggleComponent) {
	for (int i = 0; i < ToggleComponents.Num(); i++)
		if (ToggleComponents[i] == toggleComponent) {
			ToggleComponents.RemoveAt(i);
			return;
		}
}

TArray<UToggleMovementComponent*> UActivateTogglesComponent::GetToggleComponents() {
	return ToggleComponents;
}