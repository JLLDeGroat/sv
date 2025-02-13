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
#include "../../Delegates/HudDelegates.h"
#include "../Anim/CharAnimInstance.h"
#include "../../Player/Components/PawnCameraComponent.h"
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

float UDamageRecieveComponent::DoDamage(float multiplier, int damage, float impulseDamage, FRotator angleOfDamage, FVector locationOfDamage) {
	auto details = GetOwner()->GetComponentByClass<UCharacterDetailsComponent>();
	if (!details) {
		UDebugMessages::LogError(this, "No details component, cannot take damage");
		return 0;
	}

	int total = multiplier * damage;

	UDebugMessages::LogDisplay(this, "took " + FString::SanitizeFloat(total, 0) + " damage.");
	bool isDead = false;
	details->RemoveHealth(total, isDead);

	auto statusComponent = GetOwner()->GetComponentByClass<UHealthAndStatusWidgetComponent>();
	if (statusComponent) {
		UDebugMessages::LogDisplay(this, "updating health change on status widget");
		statusComponent->UpdateOnHealthChange();
		statusComponent->TakenDamage(total);
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

		if (!skeleton) {
			UDebugMessages::LogError(this, "failed to get skeletalmeshComponent on death");
			return 0;
		}

		if (details->GetCharacterControl() == ECharacterControl::CC_Player) {
			UDebugMessages::LogDisplay(this, "time dilation set to 10%");
			GetOwner()->GetWorld()->GetTimerManager().SetTimer(DeathPhysicsHandle, this, &UDamageRecieveComponent::OnDeathPhysicsHandleCallback, .6f, false);
			GetWorld()->GetWorldSettings()->TimeDilation = 0.1f;

			auto pawnCamera = GetWorld()->GetFirstPlayerController()->GetPawn()->GetComponentByClass<UPawnCameraComponent>();
			if (pawnCamera) {
				pawnCamera->SetToDeathWatchCamera(GetOwner());
			}
		}
		skeleton->SetSimulatePhysics(true);
		skeleton->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		skeleton->SetCollisionResponseToChannel(USvUtilities::GetEnvironmentChannel(), ECR_Block);

		if (angleOfDamage == FRotator::ZeroRotator)
			UDebugMessages::LogError(this, "location was zero vector, cannot add impulse on death");
		else
			skeleton->AddImpulse(angleOfDamage.Vector() * impulseDamage);

		//if (location == FVector::ZeroVector)
		//	UDebugMessages::LogError(this, "location was zero vector, cannot add impulse on death");
		//else
		//	skeleton->AddImpulse(FVector(0, 0, 100000));
		//	//skeleton->AddRadialImpulse(location, 350, impulseDamage, ERadialImpulseFalloff::RIF_Linear);

		if (details->GetCharacterControl() == ECharacterControl::CC_Player) {
			auto instance = USvUtilities::GetGameInstance(GetWorld());
			if (!instance->GetCurrentGameDataManager() || !instance->GetCurrentGameDataManager()->GetCurrentGameData()) {
				UDebugMessages::LogError(this, "failed to get game instance current game data");
				return 0.0f;
			}

			auto currentGameData = instance->GetCurrentGameDataManager()->GetCurrentGameData();
			currentGameData->SetCrewAsDead(details->GetCharacterId());
		}

		auto dropResource = owner->GetComponentByClass<UDropResourceComponent>();
		if (dropResource) {
			dropResource->AttemptToDropResource();
		}

		auto hudDelegates = UHudDelegates::GetInstance();
		if (!hudDelegates) {
			UDebugMessages::LogError(this, "failed to get hud delegates");
			return 0.0f;
		}

		hudDelegates->_OnSoldierDeath.Broadcast(owner);

		USvUtilities::AttemptToStartWinLossChecker(GetWorld());
	}
	return total;
}

void UDamageRecieveComponent::OnDeathHandleCallback() {
	GetOwner()->Destroy();
}

void UDamageRecieveComponent::OnDeathPhysicsHandleCallback() {
	GetWorld()->GetWorldSettings()->TimeDilation = 1.0f;

	auto pawnCamera = GetWorld()->GetFirstPlayerController()->GetPawn()->GetComponentByClass<UPawnCameraComponent>();
	if (pawnCamera) {
		pawnCamera->SetEndDeathWatchCamera();
	}
}