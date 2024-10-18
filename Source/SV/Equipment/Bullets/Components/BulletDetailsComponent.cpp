// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletDetailsComponent.h"

// Sets default values for this component's properties
UBulletDetailsComponent::UBulletDetailsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	BaseImpulse = 250.0f;
	PenetrationAbility = 1;
}

// Called when the game starts
void UBulletDetailsComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UBulletDetailsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

int UBulletDetailsComponent::GetBaseDamage() {
	return BaseDamage;
}
void UBulletDetailsComponent::SetBaseDamage(int damage) {
	BaseDamage = damage;
}
int UBulletDetailsComponent::GetBaseImpulse() {
	return BaseImpulse;
}
void UBulletDetailsComponent::SetBaseImpulse(int impulse) {
	BaseImpulse = impulse;
}
void UBulletDetailsComponent::SetGunShotFrom(AActor* gun) {
	GunShotFrom = gun;
}
AActor* UBulletDetailsComponent::GetGunShotFrom() {
	return GunShotFrom;
}
int UBulletDetailsComponent::GetPenetrationAbility() {
	return PenetrationAbility;
}
void UBulletDetailsComponent::SetPenetrationAbility(int ability) {
	PenetrationAbility = ability;
}
void UBulletDetailsComponent::RemoveFromPenetrationAbility(int amount) {
	PenetrationAbility -= amount;
}
void UBulletDetailsComponent::CompleteSetup() {
	bSetup = true;
}
bool UBulletDetailsComponent::GetIsSetup() {
	return bSetup;
}