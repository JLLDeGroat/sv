// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageRecieveComponent.h"
#include "CharacterDetailsComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Delegates/CharacterDelegates.h"
#include "../../Interfaces/SvChar.h"
#include "../../Interfaces/Gameplay.h"
#include "Components/SkeletalMeshComponent.h"
#include "../../GameModes/Managers/CharacterManager.h"
#include "../../Utilities/SvUtilities.h"
// Sets default values for this component's properties
UDamageRecieveComponent::UDamageRecieveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDamageRecieveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

// Called every frame
void UDamageRecieveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDamageRecieveComponent::DoDamage(float multiplier, int damage, FVector location, float impulseDamage) {
	auto details = GetOwner()->GetComponentByClass<UCharacterDetailsComponent>();
	if (!details) {
		UDebugMessages::LogError(this, "No details component, cannot take damage");
		return;
	}

	int total = multiplier * damage;

	UDebugMessages::LogDisplay(this, "took " + FString::SanitizeFloat(total, 0) + " damage.");
	bool isDead = false;
	details->RemoveHealth(total, isDead);

	if (isDead) {
		TScriptInterface<ISvChar> ownerAsCharacter = GetOwner();

		auto gameMode = USvUtilities::GetGameMode(GetOwner()->GetWorld());
		auto characterManager = gameMode->GetCharacterManager();
		characterManager->RemoveCharacter(ownerAsCharacter->GetSvCharId());

		auto owner = GetOwner();
		auto skeleton = owner->GetComponentByClass<USkeletalMeshComponent>();

		GetOwner()->GetWorld()->GetTimerManager().SetTimer(DeathHandle, this, &UDamageRecieveComponent::OnDeathHandleCallback, 3.5f, false);

		if (!skeleton)
			return UDebugMessages::LogError(this, "failed to get skeletalmeshComponent on death");

		skeleton->SetSimulatePhysics(true);
		skeleton->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

		if (location == FVector::ZeroVector)
			return UDebugMessages::LogError(this, "location was zero vector, cannot add impulse on death");

		skeleton->AddRadialImpulse(location, 350, impulseDamage, ERadialImpulseFalloff::RIF_Linear);

	}
}

void UDamageRecieveComponent::OnDeathHandleCallback() {
	GetOwner()->Destroy();
}