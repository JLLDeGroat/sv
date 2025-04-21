
#include "SurvivalDirector.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../../Utilities/SvUtilities.h"
#include "../../../../Utilities/SpawnUtilities.h"
#include "../../../../GameModes/Managers/CharacterManager.h"
#include "../../../../GameModes/Managers/DirectorManager.h"
#include "../../../../Characters/Components/AIComponent.h"
#include "../../../../Characters/Components/CharacterDetailsComponent.h"
#include "../../../../Interfaces/SvChar.h"
#include "../../../../Interfaces/Gameplay.h"
#include "Algo/Transform.h"
#include "../../Behaviours/AIMeleeRangeMove.h"
#include "../../../../Environment/Fog/FogManager.h"
#include "../../../../Environment/Fog/Components/FogSectionComponent.h"

#include "../../../../Characters/Monsters/GruntConstruct.h"
#include "../../../../Characters/Monsters/RifleConstruct.h"
#pragma optimize("", off)
USurvivalDirector::USurvivalDirector(const FObjectInitializer &ObjectInitializer)
    : UBaseDirectorGameType(ObjectInitializer)
{
}

float USurvivalDirector::GeneratePreassure()
{
    auto characterManager = USvUtilities::GetGameModeCharacterManager(GetWorld());
    TArray<TScriptInterface<ISvChar>> foundCharacters;
    TArray<TScriptInterface<ISvChar>> foundAliveCharacters;
    characterManager->GetCharacterListOfCharacterType(ECharacterControl::CC_AI, foundCharacters);

    bool hasNoActiveAi = true;

    for (TScriptInterface<ISvChar> foundChar : foundCharacters)
    {
        auto actor = foundChar->GetAsActor();
        if (actor)
        {
            auto details = actor->GetComponentByClass<UCharacterDetailsComponent>();
            if (details && !details->GetIsDead())
                foundAliveCharacters.Emplace(foundChar);
        }
    }

    for (TScriptInterface<ISvChar> enemy : foundAliveCharacters)
    {
        auto aiComponent = enemy->GetAsActor()->GetComponentByClass<UAIComponent>();
        auto bIsActive = aiComponent->GetIsActiveAi();

        if (bIsActive)
            hasNoActiveAi = false;
    }

    auto preassure = 0.0f;
    if (hasNoActiveAi)
    {
        preassure = 100;

        if (foundAliveCharacters.Num() > 0)
        {
            TArray<TScriptInterface<ISvChar>> foundPlayerCharacters;
            characterManager->GetCharacterListOfCharacterType(ECharacterControl::CC_Player, foundPlayerCharacters);
            for (TScriptInterface<ISvChar> enemy : foundAliveCharacters)
            {
                MoveThisCharTowardsPlayer(enemy);
            }
        }
    }
    return preassure;
}

void USurvivalDirector::SpawnRequiredEnemies()
{
    // survival will always spawn three if preassure is there
    auto directorManager = USvUtilities::GetGameModeDirectorManager(GetWorld());
    auto characterManager = USvUtilities::GetGameModeCharacterManager(GetWorld());

    SetPlayerAndEnemyCharacters();

    auto fogManager = USvUtilities::GetGameMode(GetWorld())->GetFogManager();

    TArray<FVector> playerLocations;
    Algo::Transform(PlayerCharacters, playerLocations,
                    [](TScriptInterface<ISvChar> svChar)
                    { return svChar->GetAsActor()->GetActorLocation(); });

    TArray<FVector> potentialSpawnLocations;
    for (UFogSectionComponent *section : fogManager->GetFogSections())
    {
        if (!section->GetIsInFog())
        {
            auto location = section->GetComponentLocation();

            for (FVector playerLoc : playerLocations)
            {
                auto distance = FVector::Dist(location, playerLoc);
                if (distance > 400 && distance < 1100)
                    potentialSpawnLocations.Emplace(location);
            }
        }
    }

    TArray<FVector> finalSpawnLocations;
    for (FVector availableLocations : directorManager->GetFloorSpawnLocations())
    {
        if (IsWithinList(availableLocations, potentialSpawnLocations))
            finalSpawnLocations.Emplace(availableLocations);
    }

    auto spawnLoop = true;
    auto spawnAmount = 3;
    while (spawnLoop)
    {
        int32 randomIndex = _randomStream.RandRange(0, finalSpawnLocations.Num() - 1);
        FVector randomVector = finalSpawnLocations[randomIndex];
        finalSpawnLocations.RemoveAt(randomIndex);

        auto world = GetWorld();
        FFunctionGraphTask::CreateAndDispatchWhenReady(
            [world, randomVector]
            {
                USpawnUtilities::SpawnEnemyOfTypeAtLocation(world, AGruntConstruct::StaticClass(), randomVector + FVector(0, 0, 75), nullptr, true);
            },
            TStatId(), nullptr, ENamedThreads::GameThread);
        UDebugMessages::LogDisplay(this, "spawned enemy");

        spawnAmount -= 1;
        if (spawnAmount <= 0)
            spawnLoop = false;
    }

    UDebugMessages::LogDisplay(this, "spawned all enemies");
}
#pragma optimize("", on)