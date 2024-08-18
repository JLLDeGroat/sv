// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupResourceComponent.h"
#include "../Anim/CharAnimInstance.h"
#include "ActionsComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Instance/Managers/CurrentGameDataManager.h"
#include "../../Environment/Components/Pickup/PickupDetailsComponent.h"
#include "../../Delegates/HudDelegates.h"



UPickupResourceComponent::UPickupResourceComponent(const FObjectInitializer& ObjectInitializer)
	: UAnimAccessComponent(ObjectInitializer) {



}

void UPickupResourceComponent::StartPickup() {
	AnimInstance->SetIsPickingUp(true);
}

void UPickupResourceComponent::AddToPickupActors(AActor* pickupsActor) {
	for (int i = 0; i < NearbyPickupActors.Num(); i++)
		if (NearbyPickupActors[i] == pickupsActor)
			return;

	NearbyPickupActors.Emplace(pickupsActor);
}
void UPickupResourceComponent::RemovePickupActor(AActor* pickupsActor) {
	for (int i = 0; i < NearbyPickupActors.Num(); i++)
		if (NearbyPickupActors[i] == pickupsActor)
			NearbyPickupActors.RemoveAt(i);

}

bool UPickupResourceComponent::HasNearbyPickupActors() {
	for (int i = 0; i < NearbyPickupActors.Num(); i++)
		if (NearbyPickupActors[i]) return true;

	return false;
}

void UPickupResourceComponent::AssignPickup() {
	auto currentGameData = USvUtilities::GetCurrentGameData(GetWorld());

	if (!currentGameData)
		return UDebugMessages::LogError(this, "failed to get current game data");

	auto owner = GetOwner();

	AActor* pickupActor = nullptr;
	for (int i = 0; i < NearbyPickupActors.Num(); i++)
		if (NearbyPickupActors[i])
			pickupActor = NearbyPickupActors[i];

	if (!pickupActor)
		return UDebugMessages::LogError(this, "failed to get pickup actor");

	auto pickupDetails = pickupActor->GetComponentByClass<UPickupDetailsComponent>();
	if (!pickupDetails)
		return UDebugMessages::LogError(this, "failed to get pickup details");

	auto resourceData = currentGameData->GetResourceData();

	auto currentResource = resourceData->GetResource(pickupDetails->GetResourceType());
	currentResource->AddToAmount(pickupDetails->GetAmount());

	UDebugMessages::LogDisplay(this, "total res: " + FString::SanitizeFloat(currentResource->GetAmount(), 0));

	RemovePickupActor(pickupActor);
	pickupActor->Destroy();

	auto actionsComponent = GetOwner()->GetComponentByClass<UActionsComponent>();
	if (!actionsComponent)
		return UDebugMessages::LogError(this, "failed to get actions component");

	actionsComponent->SendActionsToUI();
}