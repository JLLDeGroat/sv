#include "CrewMemberGen.h"
#include "../../Characters/DandD/OnBeginTargeting/KineticDrift.h"

#include "../../Characters/DandD/OnBeginTargetingThrowable/SizeMatters.h"

#include "../../Characters/DandD/OnGunFire/LeadTax.h"
#include "../../Characters/DandD/OnGunFire/TrickleDownBallistics.h"

#include "../../Characters/DandD/OnThrownTrowable/HappyEnding.h"

#include "Algo/RandomShuffle.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Instance/SvGameInstance.h"
#include "../../Instance/Managers/RouteDataManager.h"
#include "../../Instance/Managers/CurrentGameDataManager.h"
#include "../../Instance/Managers/MissionDetailsManager.h"

UCrewMemberGen::UCrewMemberGen(const FObjectInitializer &ObjectInitializer)
    : UObject(ObjectInitializer)
{
    RandomStream = FRandomStream(FMath::RandRange(0, 999999));
}
#pragma optimize("", off)
void UCrewMemberGen::GenerateCrewMembers(int amount)
{
    auto instance = USvUtilities::GetGameInstance(GetWorld());
    auto crewData = instance->GetPossibleCrewData();
    auto currentGameManager = instance->GetCurrentGameDataManager();

    auto gameData = currentGameManager->GetCurrentGameData();

    auto allFirstNames = crewData->GetFirstNames();
    auto allLastNames = crewData->GetLastNames();
    auto allBios = crewData->GetBios();

    // auto memberGen = NewObject<UCrewMemberGen>();
    auto dnds = GetStartingBaseDDClass(amount);
    // memberGen->ClearInternalFlags(EInternalObjectFlags::Async);

    for (int i = 0; i < amount; i++)
    {
        auto crewMemberId = gameData->AddCrewMember(
            allFirstNames[RandomStream.RandRange(1, allFirstNames.Num() - 1)],
            allLastNames[RandomStream.RandRange(1, allLastNames.Num() - 1)],
            allBios[RandomStream.RandRange(1, allBios.Num() - 1)],
            100,
            100);
        auto primaryId = gameData->AddPrimaryToCrew(EGun::G_PeaRifle);
        gameData->AssignPrimaryToCrew(primaryId, crewMemberId);
        gameData->AddCrewDD(crewMemberId, dnds[i]);

        if (i == 0)
        {
            auto toolAdded = gameData->AddToolToCrew(EToolType::TT_Throwable, (uint8)EThrowable::T_Grenade);
            gameData->AssignToolToCrew(toolAdded, crewMemberId);
        }
    }

    // gameData->AddPrimaryToCrew(EGun::G_PeaRifle);
    // gameData->AddPrimaryToCrew(EGun::G_PeaRifle);
    // gameData->AddPrimaryToCrew(EGun::G_PeaRifle);

    // gameData->AddToolToCrew(EToolType::TT_Throwable, (uint8)EThrowable::T_Grenade);
    // gameData->AddToolToCrew(EToolType::TT_Throwable, (uint8)EThrowable::T_Grenade);
    // gameData->AddToolToCrew(EToolType::TT_Throwable, (uint8)EThrowable::T_Grenade);
    // gameData->AddToolToCrew(EToolType::TT_Throwable, (uint8)EThrowable::T_Grenade);
    // gameData->AddToolToCrew(EToolType::TT_Throwable, (uint8)EThrowable::T_Grenade);
}

TArray<TSubclassOf<UBaseDD>> UCrewMemberGen::GetStartingBaseDDClass(int amount)
{
    TArray<TSubclassOf<UBaseDD>> arr;

    arr.Emplace(UKineticDrift::StaticClass());
    arr.Emplace(USizeMatters::StaticClass());
    arr.Emplace(ULeadTax::StaticClass());
    arr.Emplace(UTrickleDownBallistics::StaticClass());
    arr.Emplace(UHappyEnding::StaticClass());

    Algo::RandomShuffle(arr);

    if (arr.Num() > amount)
        arr.SetNum(amount);

    return arr;
}

#pragma optimize("", on)