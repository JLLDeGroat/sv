// Fill out your copyright notice in the Description page of Project Settings.


#include "DropResourceComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Utilities/GridUtilities.h"
#include "InventoryComponent.h"
#include "../../Environment/Pickups/ResourcePickup.h"
#include "../../Environment/Pickups/Components/PickupDetailsComponent.h"
// Sets default values for this component's properties
UDropResourceComponent::UDropResourceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UDropResourceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


void UDropResourceComponent::AttemptToDropResource() {
	auto randomChange = FMath::RandRange(0, 101);

	UDebugMessages::LogError(this, "THIS METHOD IS NOT DONE FULLY");
	//debug
	randomChange = 51;
	UDebugMessages::LogError(this, "random change set to always drop");
	if (randomChange > 51) {

	}

	auto inventoryComponent = GetOwner()->GetComponentByClass<UInventoryComponent>();
	if (inventoryComponent) {
		auto heldResources = inventoryComponent->GetHeldResources();
		for (const TPair<EResourceType, int>& pair : heldResources) {
			auto res = GetWorld()->SpawnActor<AResourcePickup>(GetOwner()->GetActorLocation(), FRotator::ZeroRotator, FActorSpawnParameters());

			if (!res)
				UDebugMessages::LogError(this, "failed to spawn resource drop actor");
			else {
				auto details = res->GetComponentByClass<UPickupDetailsComponent>();
				if (details) {
					details->SetResourceType(pair.Key);
					details->SetAmount(pair.Value);
				}
			}
		}

		if (inventoryComponent->GetIntel()) {
			UDebugMessages::LogError(this, "need to drop intel... ");
			auto res = GetWorld()->SpawnActor<AResourcePickup>(GetOwner()->GetActorLocation(), FRotator::ZeroRotator, FActorSpawnParameters());

			if (!res)
				UDebugMessages::LogError(this, "failed to spawn resource drop actor");
			else {
				auto details = res->GetComponentByClass<UPickupDetailsComponent>();
				if (details) {
					details->SetIsIntel();
				}
			}
		}
	}
}