#include "LeadTax.h"
#include "../../../Equipment/Bullets/Bullet.h"
#include "../../../Equipment/Bullets/Components/BulletDetailsComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

ULeadTax::ULeadTax(const FObjectInitializer &ObjectInitializer)
    : UOnGunFireBase(ObjectInitializer)
{
    Title = "Lead Tax";
    Description = "The first bullet of every turn has been replaced by a dummy, hopefully no one will notice if I scalp off the top.";
    DeviationOrDirective = EDDType::DD_Deviation;
    bCanStartOutWith = true;
}

FOnGunFireOutput ULeadTax::ActivateDD(FOnGunFireInput &input)
{
    return FOnGunFireOutput();
}
