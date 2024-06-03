// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimSpeedComponent.h"
#include "../Anim/CharAnimInstance.h"
// Sets default values for this component's properties
UAnimSpeedComponent::UAnimSpeedComponent(const FObjectInitializer& ObjectInitializer) : 
	UAnimAccessComponent(ObjectInitializer) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UAnimSpeedComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UAnimSpeedComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAnimSpeedComponent::SlowAnimation(float speed) {
	AnimInstance->UpdateAnimPlayRate(speed);
}
void UAnimSpeedComponent::ReturnToNormalAnimSpeed() {
	AnimInstance->UpdateAnimPlayRate(1.0f);
}
