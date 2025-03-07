// Fill out your copyright notice in the Description page of Project Settings.

#include "HitBoxComponent.h"
#include "../../Utilities/SvUtilities.h"
#include "ModularSkeletonComponent.h"

UHitBoxComponent::UHitBoxComponent(const FObjectInitializer &ObjectInitializer)
	: UBoxComponent(ObjectInitializer)
{

	// SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(USvUtilities::GetBulletCollisionChannel(), ECR_Overlap);
	SetCollisionResponseToChannel(ECC_GameTraceChannel5, ECR_Overlap);
	SetCollisionObjectType(USvUtilities::GetBulletCollisionObjectChannel());

	SetCanEverAffectNavigation(false);
	DamageMultiplier = 1.0f;
	Thickness = 1;

	/*SetVisibility(true);
	bHiddenInGame = false;*/
}

FVector UHitBoxComponent::GetWorldLocation()
{
	return this->GetComponentLocation();
}

void UHitBoxComponent::SetHitDimageMultiplier(float value)
{
	DamageMultiplier = value;
}

float UHitBoxComponent::GetHitDamageMultiplier()
{
	return DamageMultiplier;
}

void UHitBoxComponent::SetThickness(int thickness)
{
	Thickness = thickness;
}

int UHitBoxComponent::GetThickness()
{
	return Thickness;
}

int UHitBoxComponent::GetSpatterBackDistance()
{
	auto extent = GetScaledBoxExtent();
	return (extent.X + extent.Y) / 2;
}

void UHitBoxComponent::SetModularComponent(UModularSkeletonComponent *component)
{
	ModularComponent = component;
}
UModularSkeletonComponent *UHitBoxComponent::GetModularComponent()
{
	return ModularComponent;
}
void UHitBoxComponent::DamageModularComponent(int amount, FVector direction)
{
}

void UHitBoxComponent::AddEquipmentAsModularChild(AEquipment *equipment)
{
	if (ModularComponent)
		ModularComponent->AddChildEquipment(equipment);
}
bool UHitBoxComponent::GetIsPreferredHitLocation()
{
	return bIsPreferredTarget;
}
void UHitBoxComponent::SetIsPreferredHitLocation(bool val) {
	bIsPreferredTarget=val;
}
FVector UHitBoxComponent::GetHitLocation()
{
	return GetComponentLocation();
}
AActor* UHitBoxComponent::GetOwner(){
	return GetOwner();
}