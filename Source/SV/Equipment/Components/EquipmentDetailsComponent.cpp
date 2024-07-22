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
	Accuracy = 500;
	AccuracyDecay = 1;
	MaxAccuracyDeviation = 75;
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

void UEquipmentDetailsComponent::SetOverwatchApCost(int amount) {
	OverwatchApCost = amount;
}
int UEquipmentDetailsComponent::GetOverwatchApCost() const {
	return OverwatchApCost;
}

bool UEquipmentDetailsComponent::GetCanOverwatch() {
	return bCanOverwatch;
}
void UEquipmentDetailsComponent::SetCanOverwatch(bool val) {
	bCanOverwatch = val;
}

void UEquipmentDetailsComponent::SetIsPrimaryEquipment(bool val) {
	bIsPrimaryEquipment = val;
}
bool UEquipmentDetailsComponent::GetIsPrimaryEquipment() {
	return bIsPrimaryEquipment;
}

void UEquipmentDetailsComponent::SetBaseAccuracy(float val) {
	BaseAccuracy = val;
}
float UEquipmentDetailsComponent::GetBaseAccuracy() {
	return BaseAccuracy;
}

void UEquipmentDetailsComponent::SetAccuracy(float val) {
	Accuracy = val;
}
float UEquipmentDetailsComponent::GetAccuracy() {
	return Accuracy;
}

void UEquipmentDetailsComponent::SetAccuracyDecay(float val) {
	AccuracyDecay = val;
}
float UEquipmentDetailsComponent::GetAccuracyDecay() {
	return AccuracyDecay;
}

void UEquipmentDetailsComponent::SetMaxAccuracyDeviation(float maxDeviation) {
	MaxAccuracyDeviation = maxDeviation;
}
float UEquipmentDetailsComponent::GetMaxAccuracyDeviation() {
	return MaxAccuracyDeviation;
}
void UEquipmentDetailsComponent::RemoveFromRounds(int amount) {
	Rounds -= amount;
	if (Rounds < 0) Rounds = 0;
}
int UEquipmentDetailsComponent::GetRounds() {
	return Rounds;
}
void UEquipmentDetailsComponent::FillRounds() {
	Rounds = MaxRounds;
}
int UEquipmentDetailsComponent::GetMaxRounds() {
	return MaxRounds;
}
void UEquipmentDetailsComponent::SetMaxRounds(int maxRounds) {
	MaxRounds = maxRounds;
	Rounds = maxRounds;
}
int UEquipmentDetailsComponent::GetReloadApCost() {
	return ReloadApCost;
}
void UEquipmentDetailsComponent::SetReloadApCost(int cost) {
	ReloadApCost = cost;
}
bool UEquipmentDetailsComponent::CanReloadWeapon() {
	return Rounds < MaxRounds;
}
void UEquipmentDetailsComponent::SetGunType(EGun gunType) {
	GunType = gunType;
}
EGun UEquipmentDetailsComponent::GetGunType() {
	return GunType;
}
bool UEquipmentDetailsComponent::GetIsGun() {
	return GunType != EGun::INVALID;
}