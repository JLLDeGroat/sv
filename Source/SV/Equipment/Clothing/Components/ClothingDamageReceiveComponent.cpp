
#include "ClothingDamageReceiveComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "ClothingStatsComponent.h"

DEFINE_LOG_CATEGORY(LogClothingDamageReceiveComponent)

UClothingDamageReceiveComponent::UClothingDamageReceiveComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

float UClothingDamageReceiveComponent::DoDamage(float multiplier, int damage, float impulseDamage, FRotator angleOfDamage, FVector locationOfDamage)
{
    UDebugMessages::LogDisplay(this, "Do Damage has been hit", LogClothingDamageReceiveComponent);

    auto damageDone = damage * multiplier;

    auto thisArmourStats = GetStatsComponent();
    auto currentArmourHealth = thisArmourStats->GetHealth();
    auto armourHealthLeft = currentArmourHealth - damageDone;
    if (currentArmourHealth == 0 || armourHealthLeft < 0)
        UDebugMessages::LogDisplay(this, "armor has been broken", LogClothingDamageReceiveComponent);
    else
    {
        thisArmourStats->RemoveFromHealth(currentArmourHealth);
        UDebugMessages::LogDisplay(this, "armor has been damage " + FString::SanitizeFloat(damageDone, 2), LogClothingDamageReceiveComponent);
    }
    return damageDone;
}

float UClothingDamageReceiveComponent::DamageToMitigateFromBullet(float multiplier, int damage)
{
    auto damageDone = damage * multiplier;

    auto thisArmourStats = GetStatsComponent();
    if (!thisArmourStats)
    {
        UDebugMessages::LogError(this, "Could not get armor stats from clothing", LogClothingDamageReceiveComponent);
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