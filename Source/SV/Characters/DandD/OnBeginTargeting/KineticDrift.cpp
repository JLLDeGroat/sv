#include "KineticDrift.h"
#include "../../../Equipment/Bullets/Bullet.h"
#include "../../../Equipment/Bullets/Components/BulletDetailsComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

UKineticDrift::UKineticDrift(const FObjectInitializer &ObjectInitializer)
    : UOnBeginTargetingBase(ObjectInitializer)
{
}

FOnBeginTargetingOutput UKineticDrift::ActivateDD(FOnBeginTargetingInput &input)
{
    return FOnBeginTargetingOutput();
}