// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "AttackComponent.h"
#include "../../Equipment/Guns/PeaRifle.h"
#include "../../Equipment/Bullets/Bullet.h"
#include "../../Equipment/Guns/Components/GunFireComponent.h"
#include "../../Utilities/GridUtilities.h"
#include "../../Equipment/Bullets/Components/TravelComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Equipment/Equipment.h"
#include "../../Equipment/Components/EquipmentDetailsComponent.h"

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// ths needs major improvement in order to handle multiple equipment
	auto gun = GetOwner()->GetWorld()->SpawnActor<AEquipment>(APeaRifle::StaticClass());

	auto skeleMesh = GetOwner()->GetComponentByClass<USkeletalMeshComponent>();
	if (skeleMesh) {
		gun->AttachToComponent(skeleMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RightHandSocket"));
		gun->SetupAttachVector();

		Equipment.Emplace(gun);
	}
}


// Called every frame
void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquipmentComponent::UpdateActorVisibility(bool value) {
	for (int i = 0; i < Equipment.Num(); i++) {
		Equipment[i]->SetActorHiddenInGame(!value);
	}
}

void UEquipmentComponent::FireEquippedGun() {
	auto attackComponent = GetOwner()->GetComponentByClass<UAttackComponent>();
	if (attackComponent) {
		//TODO:
		//assuming equipped gun is an AR
		//assuming AR uses basic bullets
		auto equipment = Equipment[0];

		auto gunFire = equipment->GetComponentByClass<UGunFireComponent>();
		if (gunFire) 
			gunFire->FireAtLocation(attackComponent->GetCurrentTargetLocation());
	}
}

int UEquipmentComponent::GetActionPointsNeededToUseEquipment() {
	//TODO: 
	// set it so that we get the amount from the equipment component
	return 2;
}

TArray<AEquipment*> UEquipmentComponent::GetAllMeleeEquipment() {
	TArray<AEquipment*> foundEquipment;
	for (int i = 0; i < Equipment.Num(); i++) {
		if (!Equipment[i]) continue;

		auto equipmentDetailsComponent = Equipment[i]->GetComponentByClass<UEquipmentDetailsComponent>();
		if (!equipmentDetailsComponent) {
			UDebugMessages::LogError(this, "This equipment has no details component");
			continue;
		}

		if (equipmentDetailsComponent->GetIsMelee()) 
			foundEquipment.Emplace(Equipment[i]);
	}
	return foundEquipment;
}