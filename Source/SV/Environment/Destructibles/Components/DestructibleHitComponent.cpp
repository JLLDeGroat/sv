// Fill out your copyright notice in the Description page of Project Settings.

#include "DestructibleHitComponent.h"
#include "Field/FieldSystemComponent.h"
#include "PhysicsField/PhysicsFieldComponent.h"
#include "DestructibleDetails.h"
#include "Field/FieldSystemNodes.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "../Base/BaseChestHigh.h"

UDestructibleHitComponent::UDestructibleHitComponent(const FObjectInitializer &ObjectInitializer)
    : UActorComponent(ObjectInitializer)
{
    KillFieldSystem = CreateDefaultSubobject<UFieldSystemComponent>(TEXT("KillFieldSystem"));
}

void UDestructibleHitComponent::TestMethod(FVector loc, float f1, float f2, int32 i1)
{
    KillFieldSystem->ApplyStrainField(true, loc, f1, f2, i1);
    KillFieldSystem->ApplyRadialForce(true, loc, 5000);
}

void UDestructibleHitComponent::ComponentWasHit(float damage, FVector forwardVector, FVector hitLoc, float magnitude, float radius, int32 iterations)
{
    auto detailsComponent = GetOwner()->GetComponentByClass<UDestructibleDetails>();
    if (detailsComponent)
    {
        detailsComponent->RemoveFromHealth(damage);
        if (detailsComponent->GetIsDead())
        {
            magnitude = 5000;
            radius += 100;
            UDebugMessages::LogDisplay(this, "component has died");
        }
        else
        {
            float currentHealthPercentage = 100 - ((detailsComponent->GetHealth() / detailsComponent->GetMaxHealth()) * 100);
            UDebugMessages::LogDisplay(this, "additional1: " + FString::SanitizeFloat((detailsComponent->GetHealth() / detailsComponent->GetMaxHealth()) * 100));
            UDebugMessages::LogDisplay(this, "additional2: " + FString::SanitizeFloat(currentHealthPercentage));
            radius += currentHealthPercentage;
        }
    }

    auto owner = GetOwner<ABaseChestHigh>();
    owner->ReEnableForceOnHit();

    UDebugMessages::LogDisplay(this, "Radius: " + FString::SanitizeFloat(radius));
    KillFieldSystem->ApplyStrainField(true, hitLoc, radius, magnitude, iterations);
    // KillFieldSystem->ApplyLinearForce(true, FVector(0, 0, 1), 100);
    KillFieldSystem->ApplyRadialForce(true, hitLoc, 500);
}