#include "HappyEnding.h"
#include "../../../Equipment/Bullets/Bullet.h"
#include "../../../Equipment/Bullets/Components/BulletDetailsComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

UHappyEnding::UHappyEnding(const FObjectInitializer &ObjectInitializer)
    : UOnThrowableThrownBase(ObjectInitializer)
{
    Title = "Happy Ending";
    Description = "Explosions deal more damage, Ive put more than just death in these things.";
    DeviationOrDirective = EDDType::DD_Directive;
    bCanStartOutWith = true;
}

FOnThrowableThrownOutput UHappyEnding::ActivateDD(FOnThrowableThrownInput &input)
{
    return FOnThrowableThrownOutput();    
}