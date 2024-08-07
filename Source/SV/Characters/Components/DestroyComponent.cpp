// Fill out your copyright notice in the Description page of Project Settings.


#include "DestroyComponent.h"
#include "EquipmentComponent.h"
#include "../../Equipment/Equipment.h"

// Sets default values for this component's properties
UDestroyComponent::UDestroyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UDestroyComponent::DestroyThisActor() {
	auto owner = GetOwner();

	auto equipmentComponent = owner->GetComponentByClass<UEquipmentComponent>();

	auto allMelee = equipmentComponent->GetAllMeleeEquipment();
	for (int i = 0; i < allMelee.Num(); i++)
		if (allMelee[i])
			allMelee[i]->Destroy();

	auto primary = equipmentComponent->GetPrimaryEquipment();
	if (primary)primary->Destroy();

	owner->Destroy();
}