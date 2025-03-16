// Fill out your copyright notice in the Description page of Project Settings.

#include "HitCapsuleComponent.h"
#include "ModularSkeletonComponent.h"
#include "../../Utilities/SvUtilities.h"

UHitCapsuleComponent::UHitCapsuleComponent(const FObjectInitializer &ObjectInitializer)
	: UCapsuleComponent(ObjectInitializer)
{

	// SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionObjectType(USvUtilities::GetBulletCollisionObjectChannel());
	SetCollisionResponseToChannel(USvUtilities::GetBulletCollisionChannel(), ECR_Overlap);
	SetCollisionResponseToChannel(USvUtilities::GetBulletCollisionObjectChannel(), ECR_Overlap);
	SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	SetCanEverAffectNavigation(false);
	DamageMultiplier = 1.0f;
	Thickness = 1;

	/*SetVisibility(true);
	bHiddenInGame = false;*/
}

FVector UHitCapsuleComponent::GetWorldLocation()
{
	return this->GetComponentLocation();
}

void UHitCapsuleComponent::SetHitDimageMultiplier(float value)
{
	DamageMultiplier = value;
}

float UHitCapsuleComponent::GetHitDamageMultiplier()
{
	return DamageMultiplier;
}

int UHitCapsuleComponent::GetThickness()
{
	return Thickness;
}

void UHitCapsuleComponent::SetThickness(int thickness)
{
	Thickness = thickness;
}
int UHitCapsuleComponent::GetSpatterBackDistance()
{
	return GetScaledCapsuleRadius();
}

void UHitCapsuleComponent::SetModularComponent(UModularSkeletonComponent *component)
{
	ModularComponent = component;
}
UModularSkeletonComponent *UHitCapsuleComponent::GetModularComponent()
{
	return ModularComponent;
}
void UHitCapsuleComponent::DamageModularComponent(int amount, FVector direction)
{
	ModularComponent->RemoveHealth(amount);

	if (ModularComponent->GetIsDead())
	{
		SetGenerateOverlapEvents(false);
		ModularComponent->DestroyModularComponent(direction);
	}
}

void UHitCapsuleComponent::AddEquipmentAsModularChild(AEquipment *equipment)
{
	if (ModularComponent)
		ModularComponent->AddChildEquipment(equipment);
}
bool UHitCapsuleComponent::GetIsPreferredHitLocation()
{
	return bIsPreferredTarget;
}
 void UHitCapsuleComponent::SetIsPreferredHitLocation(bool val) {
	bIsPreferredTarget=val;
}
FVector UHitCapsuleComponent::GetHitLocation(){
	return GetComponentLocation();
}
AActor* UHitCapsuleComponent::GetOwner(){
	return GetOwner();
}