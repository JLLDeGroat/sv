// Fill out your copyright notice in the Description page of Project Settings.

#include "EndZoneGeneration.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Environment/Indicators/ExtractionIndicator.h"

UBaseGenerations *UEndZoneGeneration::Generate()
{
    LogGenerationStart();

    auto world = GetWorld();
    for (FVector location : SpawnLocations)
    {
        FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
            [world, location]
            {
                world->SpawnActor<AExtractionIndicator>(location, FRotator::ZeroRotator);
            },
            TStatId(), nullptr, ENamedThreads::GameThread);
    }

    LogGenerationEnd();
    return this;
}

UEndZoneGeneration *UEndZoneGeneration::SetSpawnLocations(TArray<FVector> locations)
{
    SpawnLocations = locations;
    return this;
}