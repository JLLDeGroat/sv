// Fill out your copyright notice in the Description page of Project Settings.


#include "GunActivationComponent.h"

// Sets default values for this component's properties
UGunActivationComponent::UGunActivationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bUseEngineSpinup = false;
}


// Called every frame
void UGunActivationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bUseEngineSpinup)
		UseEngineSpinupTick();
}

void UGunActivationComponent::BeginPlay() {
	Super::BeginPlay();
	SetComponentTickEnabled(false);
}

void UGunActivationComponent::SetupForEngineSpinUpActivation(TArray<UStaticMeshComponent*> comps) {
	bUseEngineSpinup = true;

	for (int i = 0; i < comps.Num(); i++)
		ActivationMeshAndSpeed.Add(comps[i], FMath::RandRange(350, 500) / 100.0f);
}

void UGunActivationComponent::ActivateGun() {
	SetComponentTickEnabled(true);
}
void UGunActivationComponent::DeactivateGun() {
	SetComponentTickEnabled(false);
}

void UGunActivationComponent::UseEngineSpinupTick() {
	for (const TPair<UStaticMeshComponent*, float>& pair : ActivationMeshAndSpeed) {
		auto currentRotation = pair.Key->GetRelativeRotation();
		pair.Key->SetRelativeRotation(currentRotation + FRotator(0, 0, pair.Value));
	}
}