// Fill out your copyright notice in the Description page of Project Settings.

#include "StaticSpawnerGeneration.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Environment/Spawners/StaticEnemySpawner.h"

UStaticSpawnerGeneration::UStaticSpawnerGeneration()
{
	AmountToSpawn = 10;
}
UBaseGenerations *UStaticSpawnerGeneration::Generate()
{
	LogGenerationStart();
	if (UsableLocations.Num() == 0)
	{
		UDebugMessages::LogError(this, "failed to get any usable locations");
		return this;
	}

	for (int i = 0; i < AmountToSpawn; i++)
	{
		auto random = UsableLocations[RandomStream.RandRange(0, UsableLocations.Num() - 1)];

		if (!LocationsToSpawn.Contains(random))
		{
			LocationsToSpawn.Emplace(random);
		}
	}

	auto locations = LocationsToSpawn;
	auto world = GetWorld();
	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, locations]
																			  {
		for (int i = 0; i < locations.Num(); i++) {
			FActorSpawnParameters parameters;
			world->SpawnActor<AStaticEnemySpawner>(locations[i], FRotator(0), parameters);
		} }, TStatId(), nullptr, ENamedThreads::GameThread);
		
	LogGenerationEnd();
	return this;
}

UStaticSpawnerGeneration *UStaticSpawnerGeneration::SetAmountToSpawn(int amount)
{
	AmountToSpawn = amount;
	return this;
}