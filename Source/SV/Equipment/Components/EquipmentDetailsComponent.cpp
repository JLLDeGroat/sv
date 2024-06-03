// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentDetailsComponent.h"

// Sets default values for this component's properties
UEquipmentDetailsComponent::UEquipmentDetailsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	BaseDamage = 1;
	CritMultiplier = 1.0f;
	bIsRange = false;
	bIsMelee = false;
	bIsPrimaryEquipment = false;
}


// Called when the game starts
void UEquipmentDetailsComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UEquipmentDetailsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UEquipmentDetailsComponent::GetIsMelee() const {
	return bIsMelee;
}
void UEquipmentDetailsComponent::SetIsMelee(bool val) {
	bIsMelee = val;
}

bool UEquipmentDetailsComponent::GetIsRange() const {
	return bIsRange;
}
void UEquipmentDetailsComponent::SetIsRange(bool val) {
	bIsRange = val;
}

int UEquipmentDetailsComponent::GetBaseDamage() const {
	return BaseDamage;
}
void UEquipmentDetailsComponent::SetBaseDamage(int amount) {
	BaseDamage = amount;
}

float UEquipmentDetailsComponent::GetCritMultiplier() const {
	return CritMultiplier;
}
void UEquipmentDetailsComponent::SetCritMultiplier(float amount) {
	CritMultiplier = amount;
}

void UEquipmentDetailsComponent::SetApCost(int amount) {
	ApCost = amount;
}
int UEquipmentDetailsComponent::GetApCost() const {
	return ApCost;
}

void UEquipmentDetailsComponent::SetIsPrimaryEquipment(bool val) {
	bIsPrimaryEquipment = val;
}
bool UEquipmentDetailsComponent::GetIsPrimaryEquipment() {
	return bIsPrimaryEquipment;
}