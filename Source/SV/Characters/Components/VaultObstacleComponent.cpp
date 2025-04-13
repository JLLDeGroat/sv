// Fill out your copyright notice in the Description page of Project Settings.

#include "VaultObstacleComponent.h"
#include "../Anim/CharAnimInstance.h"
#include "GridMovementComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

// Sets default values for this component's properties
UVaultObstacleComponent::UVaultObstacleComponent(const FObjectInitializer &ObjectInitializer)
	: UAnimAccessComponent(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UVaultObstacleComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UVaultObstacleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UVaultObstacleComponent::ActivateVault()
{
	AnimInstance->SetIsVaulting(true);

	auto movementComponent = GetOwner()->GetComponentByClass<UGridMovementComponent>();
	if (!movementComponent)
		return UDebugMessages::LogError(this, "could not find movement component, cannot alter vaulting Speed");

	movementComponent->SetShouldPauseMovement(true);
	movementComponent->UpdateMovementSpeed(50);
}