// Fill out your copyright notice in the Description page of Project Settings.


#include "SuicideComponent.h"
#include "DamageRecieveComponent.h"
#include "../../Effects/ConstructSuicideEffect.h"
#include "../Anim/CharAnimInstance.h"
#include "../../Runnables/PostMovementRunnable.h"
// Sets default values for this component's properties
USuicideComponent::USuicideComponent(const FObjectInitializer& ObjectInitializer)
	: UAnimAccessComponent(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void USuicideComponent::BeginSuicide() {
	AnimInstance->SetIsSuiciding(true);
}

void USuicideComponent::ActivateSuicide() {
	auto location = GetOwner()->GetActorLocation();
	auto newSuicideExplosion = GetWorld()->SpawnActor<AConstructSuicideEffect>(location, FRotator(0));
	if (newSuicideExplosion) {

	}

	GetWorld()->GetTimerManager().SetTimer(ActivationDelay, this, &USuicideComponent::ActivationDelayCallback, 3.0f, false);
}

void USuicideComponent::ActivationDelayCallback() {
	NewObject<UPostMovementRunnable>()
		->InsertVariables(GetOwner())
		->Initialise(GetOwner()->GetWorld())
		->Begin();
}