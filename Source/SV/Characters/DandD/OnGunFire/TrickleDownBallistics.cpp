#include "TrickleDownBallistics.h"
#include "../../../Equipment/Bullets/Bullet.h"
#include "../../../Equipment/Bullets/Components/BulletDetailsComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

UTrickleDownBallistics::UTrickleDownBallistics(const FObjectInitializer &ObjectInitializer)
    : UOnGunFireBase(ObjectInitializer)
{
    Title = "Trickle Down Ballistics";
    Description = "Bullets will do more damage if shot from a higher elevation than the target, easier head shots.";
    DeviationOrDirective = EDDType::DD_Directive;
    bCanStartOutWith = true;
}

FOnGunFireOutput UTrickleDownBallistics::ActivateDD(FOnGunFireInput &input)
{
    auto initialZ = input.GetStartLocation().Z;
    auto desiredZ = input.GetDesiredLocation().Z;

    if (initialZ - desiredZ > 200)
    {
        auto bulletDetails = input.GetBullet()->GetComponentByClass<UBulletDetailsComponent>();
        if (bulletDetails)
        {
            UDebugMessages::LogDisplay(this, "TrickeDownBallistics activated");
            auto baseDam = bulletDetails->GetBaseDamage();
            bulletDetails->SetBaseDamage(baseDam * 2);
        }
    }

    return FOnGunFireOutput();
}
