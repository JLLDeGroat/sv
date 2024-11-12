// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterDetailsComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

// Sets default values for this component's properties
UCharacterDetailsComponent::UCharacterDetailsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	CharacterId = FGuid::NewGuid();
}


// Called when the game starts
void UCharacterDetailsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UCharacterDetailsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterDetailsComponent::SetCharacterControl(ECharacterControl characterControl) {
	ControlType = characterControl;
}
ECharacterControl UCharacterDetailsComponent::GetCharacterControl() const {
	return ControlType;
}

void UCharacterDetailsComponent::SetHealth(int health) {
	Health = health;
	MaxHealth = health;
}
int UCharacterDetailsComponent::GetHealth() const {
	return Health;
}

void UCharacterDetailsComponent::SetMaxHealth(int health) {
	MaxHealth = health;
}
int UCharacterDetailsComponent::GetMaxHealth() const {
	return MaxHealth;
}

void UCharacterDetailsComponent::RemoveHealth(int health, bool& isDead) {
	Health -= health;
	if (Health <= 0) {
		Health = 0;
		isDead = true;
	}
}

void UCharacterDetailsComponent::AddHealth(int health) {
	Health += health;
	if (Health > MaxHealth) {
		Health = MaxHealth;
	}
}

void UCharacterDetailsComponent::RefreshOnNewTurn() {
	MovementPoints = MaxMovementPoints;
	ActionPoints = MaxActionPoints;
	UDebugMessages::LogDisplay(this, "refreshed movement and action points");
}

void UCharacterDetailsComponent::RemoveMovementPoints(int amount) {
	MovementPoints -= amount;
	if (MovementPoints < 0) MovementPoints = 0;
}
void UCharacterDetailsComponent::AddMovementPoints(int amount) {
	MovementPoints += amount;
	if (MovementPoints > MaxMovementPoints) MovementPoints = MaxMovementPoints;
}
int UCharacterDetailsComponent::GetMovementPoints() const {
	return MovementPoints;
}

void UCharacterDetailsComponent::RemoveMaxMovementPoints(int amount) {
	MaxMovementPoints -= amount;
	if (MaxMovementPoints < 0) MaxMovementPoints = 0;
}
void UCharacterDetailsComponent::AddMaxMovementPoints(int amount) {
	MaxMovementPoints += amount;
}
int UCharacterDetailsComponent::GetMaxMovementPoints() const {
	return MaxMovementPoints;
}

void UCharacterDetailsComponent::RemoveActionPoints(int amount) {
	ActionPoints -= amount;
	if (ActionPoints < 0) ActionPoints = 0;
}
void UCharacterDetailsComponent::AddActionPoints(int amount) {
	ActionPoints += amount;
	if (ActionPoints > MaxActionPoints) ActionPoints = MaxActionPoints;
}
int UCharacterDetailsComponent::GetActionPoints() const {
	return ActionPoints;
}

void UCharacterDetailsComponent::RemoveMaxActionPoints(int amount) {
	MaxActionPoints -= amount;
}
void UCharacterDetailsComponent::AddMaxActionPoints(int amount) {
	MaxActionPoints += amount;
	if (MaxActionPoints < 0) MaxActionPoints = 0;
}
int UCharacterDetailsComponent::GetMaxActionPoints() const {
	return MaxActionPoints;
}

void UCharacterDetailsComponent::SetCanVault(bool value) {
	bCanVault = value;
}
bool UCharacterDetailsComponent::GetCanVault() {
	return bCanVault;
}

void UCharacterDetailsComponent::SetCharacterName(FString name) {
	CharacterName = name;
}
FString UCharacterDetailsComponent::GetCharacterName() {
	return CharacterName;
}
float UCharacterDetailsComponent::GetHealthAsPercentage() const {
	return (float)Health / (float)MaxHealth;
}

FGuid UCharacterDetailsComponent::GetCharacterId() const {
	return CharacterId;
}
void UCharacterDetailsComponent::SetCharacterId(FGuid guid) {
	CharacterId = guid;
}

bool UCharacterDetailsComponent::GetIsDead() {
	return Health <= 0;
}
ETargetIcon UCharacterDetailsComponent::GetTargetIcon() {
	return TargetIcon;
}
void UCharacterDetailsComponent::SetTargetIcon(ETargetIcon targetIcon) {
	TargetIcon = targetIcon;
}