// Fill out your copyright notice in the Description page of Project Settings.


#include "SkipObstacleComponent.h"
#include "../Anim/CharAnimInstance.h"
#include "GridMovementComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

// Sets default values for this component's properties
USkipObstacleComponent::USkipObstacleComponent(const FObjectInitializer& ObjectInitializer)
	: UAnimAccessComponent(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void USkipObstacleComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void USkipObstacleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USkipObstacleComponent::ActivateSkip() {
	AnimInstance->SetIsSkipping(true);
	
	auto movementComponent = GetOwner()->GetComponentByClass<UGridMovementComponent>();
	if (!movementComponent)
		return UDebugMessages::LogError(this, "could not find movement component, cannot alter skipping Speed");

	movementComponent->UpdateMovementSpeed(75);
}