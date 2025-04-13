// Fill out your copyright notice in the Description page of Project Settings.

#include "MiscellaneousGenerations.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Environment/Spawners/StaticEnemySpawner.h"
#include "../../Environment/Destructibles/ChestHighs/ChestHighRock.h"

#pragma optimize("", off)
UBaseGenerations *UMiscellaneousGenerations::Generate()
{
    LogGenerationStart();
    ChanceIncrement = 5;
    ChanceDecrement = 15;

    for (FVector usableLoc : UsableLocations)
    {
        if (usableLoc.Z == 0)
        {
            if (ShouldGenerate())
            {
                SpawningChestHighsLocations.Emplace(usableLoc);
                DecrementChance();
            }
            else
            {
                IncrementChance();
            }
        }
    }

    SetRequiredLocations(SpawningChestHighsLocations);
    TotalUsedLocations = SpawningChestHighsLocations;

    auto world = GetWorld();
    for (FVector toSpawn : SpawningChestHighsLocations)
    {
        FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
            [world, toSpawn]
            {
                world->SpawnActor<AChestHighRock>(toSpawn, FRotator::ZeroRotator);
            },
            TStatId(), nullptr, ENamedThreads::GameThread);
    }
    LogGenerationEnd();
    return this;
}
#pragma optimize("", on)
