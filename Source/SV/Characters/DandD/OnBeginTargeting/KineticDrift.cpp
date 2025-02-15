#include "KineticDrift.h"
#include "../../../Equipment/Bullets/Bullet.h"
#include "../../../Equipment/Bullets/Components/BulletDetailsComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

UKineticDrift::UKineticDrift(const FObjectInitializer &ObjectInitializer)
    : UOnBeginTargetingBase(ObjectInitializer)
{
    Title = "Kinetic Drift";
    Description = "It gets harder to hold my weapon the further I move and that this point, who cares if I miss... I just need a sit down.";
    DeviationOrDirective = EDDType::DD_Deviation;
    bCanStartOutWith = true;
}

FOnBeginTargetingOutput UKineticDrift::ActivateDD(FOnBeginTargetingInput &input)
{
    return FOnBeginTargetingOutput();
}