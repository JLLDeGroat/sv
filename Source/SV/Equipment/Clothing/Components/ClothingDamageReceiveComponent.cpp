
#include "ClothingDamageReceiveComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "ClothingStatsComponent.h"
#include "../../../Characters/Components/HealthAndStatusWidgetComponent.h"

DEFINE_LOG_CATEGORY(SVLogClothingDamageReceiveComponent)

UClothingDamageReceiveComponent::UClothingDamageReceiveComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}
#pragma optimize("", off)
float UClothingDamageReceiveComponent::DoDamage(float multiplier, int damage, float impulseDamage, FRotator angleOfDamage, FVector locationOfDamage)
{
    auto damageDone = damage * multiplier;

    auto thisArmourStats = GetStatsComponent();
    auto currentArmourHealth = thisArmourStats->GetHealth();
    auto armourHealthLeft = currentArmourHealth - damageDone;

    if (damageDone > 0)
    {
        auto statusComponent = GetOwner()->GetOwner()->GetComponentByClass<UHealthAndStatusWidgetComponent>();
        if (statusComponent)
        {
            UDebugMessages::LogDisplay(this, GetOwner()->GetName() + " updating health change on status widget", SVLogClothingDamageReceiveComponent);
            statusComponent->UpdateOnHealthChange();
        }
        else
        {
            UDebugMessages::LogError(this, "No status component, cannot update health change on status widget", SVLogClothingDamageReceiveComponent);
        }

        if (currentArmourHealth == 0 || armourHealthLeft < 0)
        {
            UDebugMessages::LogDisplay(this, GetOwner()->GetName() + " armor has been broken, health is " + FString::SanitizeFloat(thisArmourStats->GetHealth(), 2), SVLogClothingDamageReceiveComponent);
        }
        else
        {
            thisArmourStats->RemoveFromHealth(damageDone);
            UDebugMessages::LogDisplay(this, GetOwner()->GetName() + " armor has been damaged for " + FString::SanitizeFloat(damageDone, 2) + " leaving " + FString::SanitizeFloat(armourHealthLeft, 2) + " health left", SVLogClothingDamageReceiveComponent);
        }
    }
    else
        UDebugMessages::LogDisplay(this, GetOwner()->GetName() + " armor has not been damaged, health is " + FString::SanitizeFloat(thisArmourStats->GetHealth(), 2), SVLogClothingDamageReceiveComponent);
    return damageDone;
}
#pragma optimize("", on)
float UClothingDamageReceiveComponent::DamageToMitigateFromBullet(float multiplier, int damage)
{
    auto damageDone = damage * multiplier;

    auto thisArmourStats = GetStatsComponent();
    if (!thisArmourStats)
    {
        UDebugMessages::LogError(this, "Could not get armor stats from clothing", SVLogClothingDamageReceiveComponent);
        return 0;
    }

    if (thisArmourStats->GetHealth() > 0)
        return thisArmourStats->GetDamageMitigation() * damage;
    else
        return 0;
}

UClothingStatsComponent *UClothingDamageReceiveComponent::GetStatsComponent()
{
    auto thisArmourStats = GetOwner()->GetComponentByClass<UClothingStatsComponent>();
    return thisArmourStats;
}