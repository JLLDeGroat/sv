// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyGeneration.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Characters/Monsters/ZombieGrunt.h"
UEnemyGeneration *UEnemyGeneration::SetPodAmountAndTier(int amount, int maxTier)
{
	PodAmount = amount;
	PodTierAmount = maxTier;
	return this;
}
#pragma optimize("", off)
UBaseGenerations *UEnemyGeneration::Generate()
{
	LogGenerationStart();
	int spawnedWalkingZombies = 0;
	int maxSpawnedWalkingZombies = TotalWalkingZombies;

	int spawnedPods = 0;

	TArray<FVector> foundLocations;
	for (int i = 0; i < UsableLocations.Num(); i++)
	{
		// spawn pods
		auto podLocations = GeneratePodSpawnLocations(UsableLocations[i]);

		if (IsValidPodLocations(podLocations) && spawnedPods < PodAmount)
		{
			auto random = RandomStream.RandRange(1, 2002);
			if (random < 500)
			{
				for (int32 j = podLocations.Num() - 1; j > 0; --j)
				{
					int32 indexSwap = RandomStream.RandRange(0, j);
					if (j != indexSwap)
						podLocations.Swap(j, indexSwap);
				}

				int spawnedInPod = 0;
				for (int x = 0; x < podLocations.Num(); x++)
				{
					PodLocations.Emplace(podLocations[x]);
					if (IsWithinList(podLocations[x], UsableLocations) && spawnedInPod < 3)
					{
						EnemySpawnLocations.Emplace(podLocations[x]);
						spawnedInPod += 1;
					}
				}

				spawnedPods += 1;
			}
		}
	}

	UDebugMessages::LogDisplay(this, "spawwning enemies: " + FString::SanitizeFloat(EnemySpawnLocations.Num()));

	for (int i = 0; i < EnemySpawnLocations.Num(); i++)
	{
		UDebugMessages::LogDisplay(this, "Spawned Enemy at: " + EnemySpawnLocations[i].ToString());
		auto loc = EnemySpawnLocations[i];
		loc.Z += 100;
		auto world = GetWorld();
		FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
			[world, loc]
			{
				auto actor = world->SpawnActor<AZombieGrunt>(loc, FRotator::ZeroRotator);
			},
			TStatId(), nullptr, ENamedThreads::GameThread);
	}

	LogGenerationEnd();
	return this;
}

TArray<FVector> UEnemyGeneration::GeneratePodSpawnLocations(FVector startLocation)
{
	TArray<FVector> locations;
	for (int x = -3; x < 3; x++)
	{
		auto xAmount = x * 100;
		for (int y = -3; y < 3; y++)
		{
			int yAmount = y * 100;

			locations.Emplace(startLocation + FVector(xAmount, yAmount, 0));
		}
	}
	return locations;
}

bool UEnemyGeneration::IsValidPodLocations(TArray<FVector> podLocs)
{
	for (int i = 0; i < podLocs.Num(); i++)
	{
		auto thisLoc = podLocs[i];

		if (IsTooCloseToSpawnZone(podLocs[i], 800))
			return false;

		if (IsWithinList(podLocs[i], AllEnemyLocations))
			return false;

		if (IsWithinList(podLocs[i], PodLocations))
			return false;
	}

	return true;
}

bool UEnemyGeneration::IsTooCloseToSpawnZone(FVector location, float allowedDistance)
{
	auto random = StartZones[RandomStream.RandRange(0, StartZones.Num() - 1)];
	auto dist = FVector::Dist(location, random);

	return dist <= allowedDistance;
}

UEnemyGeneration *UEnemyGeneration::SetTotalWalkingZombies(int amount)
{
	TotalWalkingZombies = amount;
	return this;
}

bool UEnemyGeneration::IsWithinList(FVector loc, TArray<FVector> location)
{
	for (int i = 0; i < location.Num(); i++)
		if (location[i] == loc)
			return true;

	return false;
}

bool UEnemyGeneration::IsTooCloseToOtherSpawns(FVector loc, TArray<FVector> location, float distance)
{
	for (int i = 0; i < location.Num(); i++)
	{
		auto dist = FVector::Dist(location[i], loc);
		if (dist < distance)
			return true;
	}

	return false;
}
#pragma optimize("", on)