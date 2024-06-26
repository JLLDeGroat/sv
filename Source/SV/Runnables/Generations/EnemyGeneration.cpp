// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyGeneration.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Characters/Monsters/ZombieGrunt.h"

#pragma optimize("", off)
UBaseGenerations* UEnemyGeneration::Generate() {

	int spawnedWalkingZombies = 0;
	int maxSpawnedWalkingZombies = TotalWalkingZombies;

	TArray<FVector> foundLocations;
	for (int i = 0; i < AllowedSpots.Num(); i++) {

		if (IsTooCloseToSpawnZone(AllowedSpots[i], 800))
			continue;

		if (IsWithinList(AllowedSpots[i], AllEnemyLocations))
			continue;

		if (spawnedWalkingZombies >= maxSpawnedWalkingZombies)
			break;

		if (IsTooCloseToOtherSpawns(AllowedSpots[i], foundLocations, 750))
			continue;

		auto random = RandomStream.RandRange(1, 2002);
		if (random < 500) {
			foundLocations.Emplace(AllowedSpots[i]);
			spawnedWalkingZombies += 1;
		}
	}

	if (foundLocations.Num() < maxSpawnedWalkingZombies) {
		UDebugMessages::LogError(this, "failed to spawn all zombies");
	}
	else {
		UDebugMessages::LogDisplay(this, "spawned correct amount of zombies");
	}

	for (int i = 0; i < foundLocations.Num(); i++) {
		UDebugMessages::LogDisplay(this, foundLocations[i].ToString());
		auto loc = foundLocations[i];
		loc.Z += 100;
		auto world = GetWorld();
		FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, loc] {
			auto actor = world->SpawnActor<AZombieGrunt>(loc, FRotator::ZeroRotator);
			}, TStatId(), nullptr, ENamedThreads::GameThread);
	}

	return this;
}


bool UEnemyGeneration::IsTooCloseToSpawnZone(FVector location, float allowedDistance) {
	auto random = StartZones[RandomStream.RandRange(0, StartZones.Num() - 1)];
	auto dist = FVector::Dist(location, random);

	return dist <= allowedDistance;
}

UEnemyGeneration* UEnemyGeneration::SetTotalWalkingZombies(int amount) {
	TotalWalkingZombies = amount;
	return this;
}

bool UEnemyGeneration::IsWithinList(FVector loc, TArray<FVector> location) {
	for (int i = 0; i < location.Num(); i++)
		if (location[i] == loc)
			return true;

	return false;
}

bool UEnemyGeneration::IsTooCloseToOtherSpawns(FVector loc, TArray<FVector> location, float distance) {
	for (int i = 0; i < location.Num(); i++) {
		auto dist = FVector::Dist(location[i], loc);
		if (dist < distance) return true;
	}

	return false;
}

#pragma optimize("", on)