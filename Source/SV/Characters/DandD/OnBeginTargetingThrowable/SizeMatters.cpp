#include "SizeMatters.h"
#include "../../../Equipment/Bullets/Bullet.h"
#include "../../../Equipment/Bullets/Components/BulletDetailsComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

USizeMatters::USizeMatters(const FObjectInitializer &ObjectInitializer)
    : UOnBeginTargetingThrowableBase(ObjectInitializer)
{
    Title = "Size Matters";
    Description = "Explosions radius is halfed, think I screwed the concoction somewhat.";
    DeviationOrDirective = EDDType::DD_Deviation;
    bCanStartOutWith = true;
}

FOnBeginTargetingThrowableOutput USizeMatters::ActivateDD(FOnBeginTargetingThrowableInput &input)
{
    return FOnBeginTargetingThrowableOutput();
}