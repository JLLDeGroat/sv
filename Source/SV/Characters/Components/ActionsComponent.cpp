// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionsComponent.h"
#include "TargetingComponent.h"
#include "EquipmentComponent.h"
#include "ThrowableComponent.h"
#include "PickupResourceComponent.h"
#include "../../Equipment/Equipment.h"
#include "../../Equipment/Components/EquipmentDetailsComponent.h"
#include "../../Delegates/HudDelegates.h"
#include "../../Characters/Components/CharacterDetailsComponent.h"
#include "../../Characters/Components/ActivateTogglesComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

// Sets default values for this component's properties
UActionsComponent::UActionsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}
// Called when the game starts
void UActionsComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UActionsComponent::SetCanExtract(bool val) {
	bCanExtract = val;
}
bool UActionsComponent::GetCanExtract() {
	return bCanExtract;
}

void UActionsComponent::SendActionsToUI() {
	UDebugMessages::LogDisplay(this, "SendActionsToUI");
	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "failed to get hud delegates, returning");

	hudDelegates->_ResetActionIcons.Broadcast();

	auto currentCharacterDetails = GetOwner()->GetComponentByClass<UCharacterDetailsComponent>();

	if (!currentCharacterDetails)
		return UDebugMessages::LogError(this, "could not find character details");

	auto equipmentComponent = GetOwner()->GetComponentByClass<UEquipmentComponent>();
	if (equipmentComponent) {
		auto primaryEquipment = equipmentComponent->GetPrimaryEquipment();
		if (primaryEquipment) {
			auto primaryEquipmentDetails = primaryEquipment->GetComponentByClass<UEquipmentDetailsComponent>();

			if (primaryEquipmentDetails) {
				if (primaryEquipmentDetails->CanReloadWeapon())
					hudDelegates->_AddActionIconToHud.Broadcast(EActionType::AT_Reload, "R");

				if (currentCharacterDetails->GetActionPoints() >= primaryEquipmentDetails->GetOverwatchApCost() &&
					primaryEquipmentDetails->GetCanOverwatch())
					hudDelegates->_AddActionIconToHud.Broadcast(EActionType::AT_Overwatch, "O");
			}

			auto targetingComponent = GetOwner()->GetComponentByClass<UTargetingComponent>();
			if (targetingComponent && targetingComponent->GetCurrentTargetData().Num() > 0 &&
				primaryEquipmentDetails->GetRounds() > 0)
				hudDelegates->_AddActionIconToHud.Broadcast(EActionType::AT_Shoot, "F");
		}
	}

	auto activateToggleComponent = GetOwner()->GetComponentByClass<UActivateTogglesComponent>();
	if (activateToggleComponent) {
		auto toggles = activateToggleComponent->GetToggleComponents();
		for (int i = 0; i < toggles.Num(); i++) {
			if (toggles[i]) {
				hudDelegates->_AddActionIconToHud.Broadcast(EActionType::AT_Interact, "");
			}
		}
	}


	auto throwableComponent = GetOwner()->GetComponentByClass<UThrowableComponent>();
	if (throwableComponent && throwableComponent->GetThrowableAmount(EThrowable::T_Grenade))
		hudDelegates->_AddActionIconToHud.Broadcast(EActionType::AT_Grenade, "G");

	if (bCanExtract) {
		hudDelegates->_AddActionIconToHud.Broadcast(EActionType::AT_Extract, "E");
	}

	auto pickups = GetOwner()->GetComponentByClass<UPickupResourceComponent>();
	if (pickups && pickups->HasNearbyPickupActors())
		hudDelegates->_AddActionIconToHud.Broadcast(EActionType::AT_Pickup, "P");

	hudDelegates->_AddActionIconToHud.Broadcast(EActionType::AT_Sleep, "9");
	hudDelegates->_AddSoldierToCharacterDetailsWidget.Broadcast(GetOwner());
}