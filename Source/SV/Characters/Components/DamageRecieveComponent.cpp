// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageRecieveComponent.h"
#include "CharacterDetailsComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Delegates/CharacterDelegates.h"
#include "../../Interfaces/SvChar.h"
#include "../../Interfaces/Gameplay.h"
#include "Components/SkeletalMeshComponent.h"
#include "HealthAndStatusWidgetComponent.h"
#include "../../GameModes/Managers/CharacterManager.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Instance/Managers/CurrentGameDataManager.h"
#include "../../Instance/SvGameInstance.h"
#include "../../Runnables/Checkers/WinLossCheckerRunnable.h"
#include "../Anim/CharAnimInstance.h"
#include "DropResourceComponent.h"
// Sets default values for this component's properties
UDamageRecieveComponent::UDamageRecieveComponent(const FObjectInitializer& ObjectInitializer)
	: UAnimAccessComponent(ObjectInitializer)
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

	auto statusComponent = GetOwner()->GetComponentByClass<UHealthAndStatusWidgetComponent>();
	if (statusComponent) {
		UDebugMessages::LogDisplay(this, "updating health change on status widget");
		statusComponent->UpdateOnHealthChange();
	}

	AnimInstance->SetIsTakenDamage(true);

	UDebugMessages::LogDisplay(this, "health is now " + FString::SanitizeFloat(details->GetHealth()));

	if (isDead) {
		TScriptInterface<ISvChar> ownerAsCharacter = GetOwner();

		auto gameMode = USvUtilities::GetGameMode(GetOwner()->GetWorld());
		auto characterManager = gameMode->GetCharacterManager();
		characterManager->RemoveCharacter(details->GetCharacterId());

		auto owner = GetOwner();
		auto skeleton = owner->GetComponentByClass<USkeletalMeshComponent>();

		GetOwner()->GetWorld()->GetTimerManager().SetTimer(DeathHandle, this, &UDamageRecieveComponent::OnDeathHandleCallback, 3.5f, false);

		if (!skeleton)
			return UDebugMessages::LogError(this, "failed to get skeletalmeshComponent on death");

		skeleton->SetSimulatePhysics(true);
		skeleton->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

		if (location == FVector::ZeroVector)
			UDebugMessages::LogError(this, "location was zero vector, cannot add impulse on death");
		else
			skeleton->AddRadialImpulse(location, 350, impulseDamage, ERadialImpulseFalloff::RIF_Linear);

		if (details->GetCharacterControl() == ECharacterControl::CC_Player) {
			auto instance = USvUtilities::GetGameInstance(GetWorld());
			if (!instance->GetCurrentGameDataManager() || !instance->GetCurrentGameDataManager()->GetCurrentGameData())
				return UDebugMessages::LogError(this, "failed to get game instance current game data");

			auto currentGameData = instance->GetCurrentGameDataManager()->GetCurrentGameData();
			currentGameData->SetCrewAsDead(details->GetCharacterId());
		}

		auto dropResource = owner->GetComponentByClass<UDropResourceComponent>();
		if(dropResource){
			dropResource->AttemptToDropResource();
		}


		WinLossRunnable = NewObject<UWinLossCheckerRunnable>()
			->Initialise(GetWorld())
			->Begin();
	}
}

void UDamageRecieveComponent::OnDeathHandleCallback() {
	GetOwner()->Destroy();
}