// Fill out your copyright notice in the Description page of Project Settings.

#include "ClothingHitBoxComponent.h"
#include "../../../Utilities/SvUtilities.h"
#include "../../../Characters/Components/ModularSkeletonComponent.h"
#include "ClothingStatsComponent.h"

UClothingHitBoxComponent::UClothingHitBoxComponent(const FObjectInitializer &ObjectInitializer) : UBoxComponent(ObjectInitializer)
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

FVector UClothingHitBoxComponent::GetWorldLocation()
{
    return this->GetComponentLocation();
}

void UClothingHitBoxComponent::SetHitDimageMultiplier(float value)
{
    DamageMultiplier = value;
}

float UClothingHitBoxComponent::GetHitDamageMultiplier()
{
    return DamageMultiplier;
}

void UClothingHitBoxComponent::SetThickness(int thickness)
{
    Thickness = thickness;
}
#pragma optimize("", off)
int UClothingHitBoxComponent::GetThickness()
{
    auto stats = GetOwner()->GetComponentByClass<UClothingStatsComponent>();
    if (stats && stats->GetHealth() <= 0)
        return 0;

    return Thickness;
}
#pragma optimize("", on)
int UClothingHitBoxComponent::GetSpatterBackDistance()
{
    auto extent = GetScaledBoxExtent();
    return (extent.X + extent.Y) / 2;
}

void UClothingHitBoxComponent::SetModularComponent(UModularSkeletonComponent *component)
{
    ModularComponent = component;
}
UModularSkeletonComponent *UClothingHitBoxComponent::GetModularComponent()
{
    return ModularComponent;
}
void UClothingHitBoxComponent::DamageModularComponent(int amount, FVector direction)
{
}

void UClothingHitBoxComponent::AddEquipmentAsModularChild(AEquipment *equipment)
{
    if (ModularComponent)
        ModularComponent->AddChildEquipment(equipment);
}
bool UClothingHitBoxComponent::GetIsPreferredHitLocation()
{
    return bIsPreferredTarget;
}
void UClothingHitBoxComponent::SetIsPreferredHitLocation(bool val)
{
    bIsPreferredTarget = val;
}
FVector UClothingHitBoxComponent::GetHitLocation()
{
    return GetComponentLocation();
}
AActor *UClothingHitBoxComponent::GetOwningActor()
{
    return GetOwner();
}
UPrimitiveComponent *UClothingHitBoxComponent::GetAsPrimitiveComponent()
{
    return this;
}