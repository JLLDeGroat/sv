
#include "SVDirector.h"
#include "../../../GameModes/Managers/DirectorManager.h"
#include "../../../GameModes/Managers/TurnManager.h"
#include "../../../Utilities/SvUtilities.h"
#include "../../../Data/Game/FCurrentGameData.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Algo/Accumulate.h"

#include "GameTypes/ExterminateDirector.h"
#include "GameTypes/ExtractionDirector.h"
#include "GameTypes/SurvivalDirector.h"
#include "GameTypes/TraversalDirector.h"

USVDirector::USVDirector(const FObjectInitializer &ObjectInitializer)
    : UObject(ObjectInitializer)
{
}
#pragma optimize("", off)
void USVDirector::ActivateSvDirector()
{
    auto directorManager = USvUtilities::GetGameModeDirectorManager(GetWorld());
    auto currentGameData = USvUtilities::GetCurrentGameData(GetWorld());

    auto missionData = currentGameData->GetCurrentMission()->GetMissionDetails();

    missionData->GetMissionType();

    if (missionData->GetTurn() <= 3)
    {
        UDebugMessages::LogError(this, "Turn is between 1 and 3, were not processing during this time");
        return;
    }

    UBaseDirectorGameType *gameTypeDirector = nullptr;

    switch (missionData->GetMissionType())
    {
    // case EMissionType::MT_Exterminate:
    //     gameTypeDirector = NewObject<UExterminateDirector>(this);
    //     break;
    // case EMissionType::MT_Traverse:
    //     gameTypeDirector = NewObject<UTraversalDirector>(this);
    //     break;
    // case EMissionType::MT_Survive:
    //     gameTypeDirector = NewObject<USurvivalDirector>(this);
    //     break;
    // case EMissionType::MT_Extraction:
    //     gameTypeDirector = NewObject<UExtractionDirector>(this);
    //     break;
    case EMissionType::MT_Exterminate:
    case EMissionType::MT_Traverse:
    case EMissionType::MT_Survive:
    case EMissionType::MT_Extraction:
        gameTypeDirector = NewObject<USurvivalDirector>(this);
    default:
        UDebugMessages::LogError(this, "failed to get mission type director");
    }

    if (gameTypeDirector)
    {
        UDebugMessages::LogDisplay(this, "found gameType director " + gameTypeDirector->GetName());
        auto preassureGenerated = gameTypeDirector->GeneratePreassure();
        UDebugMessages::LogDisplay(this, "preassure " + FString::SanitizeFloat(preassureGenerated));
        if (preassureGenerated >= 100)
        {
            gameTypeDirector->SpawnRequiredEnemies();
        }

        gameTypeDirector->ClearInternalFlags(EInternalObjectFlags::Async);
    }

    // auto allCrewStats = directorManager->GetAllCrewStats();

    // auto accumulatedNoise = Algo::Accumulate(
    //     allCrewStats, 0.0f,
    //     [](float Sum, const FDirectorCrewStats *Stat)
    //     {
    //         return Sum + Stat->GetNoise();
    //     });

    // UDebugMessages::LogDisplay(this, "Total noise " + FString::SanitizeFloat(accumulatedNoise));
}
#pragma optimize("", off)