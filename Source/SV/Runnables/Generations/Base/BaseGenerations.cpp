// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseGenerations.h"
#include "../../../World/WorldGridItemActor.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Utilities/LoadBpActorTask.h"

UBaseGenerations::UBaseGenerations()
{
}

UBaseGenerations *UBaseGenerations::SetupGeneration(UWorld *world, FRandomStream random, TArray<FVector> allowedSpots)
{
	World = world;
	RandomStream = random;
	UsableLocations = allowedSpots;
	return this;
}

UBaseGenerations *UBaseGenerations::Generate()
{
	UDebugMessages::LogError(this, "GENERATION NOT WORKING, NO IMPLEMENTATION");
	return this;
}

void UBaseGenerations::SetChanceIncrement(int amount)
{
	ChanceIncrement += amount;
}
void UBaseGenerations::SetChanceDecrement(int amount)
{
	ChanceDecrement -= amount;
}

bool UBaseGenerations::ShouldGenerate()
{
	return CurrentChance > RandomStream.RandRange(1, 1001);
}

void UBaseGenerations::SetRequiredSpots(TArray<FVector> requiredSpots)
{
	RequiredLocations = requiredSpots;
}

bool UBaseGenerations::CanFitInLocation(FVector loc)
{
	auto requiredLocs = CreateRequiredLocations(loc);

	/*for (int i = 0; i < requiredLocs.Num(); i++)
		if (!IsWithinAllowedList(requiredLocs[i]) ||
			IsWithinCurrentUsedList(requiredLocs[i]) ||
			IsWithinRequiredBufferSpots(requiredLocs[i]))
			return false;*/

	for (int i = 0; i < requiredLocs.Num(); i++)
		if (!IsWithinList(UsableLocations, requiredLocs[i]) ||
			IsWithinList(TotalUsedLocations, requiredLocs[i]))
			return false;

	return true;
}

void UBaseGenerations::IncrementChance()
{
	CurrentChance += ChanceIncrement;
}
void UBaseGenerations::DecrementChance()
{
	CurrentChance -= ChanceDecrement;
}

void UBaseGenerations::SetRequiredLocations(TArray<FVector> locations)
{
	RequiredLocations = locations;
}
void UBaseGenerations::SetRequiredBufferLocations(TArray<FVector> locations)
{
	RequiredBufferLocations = locations;
}

TArray<FVector> UBaseGenerations::GetRequiredLocations()
{
	return RequiredLocations;
}
TArray<FVector> UBaseGenerations::GetRequiredBufferLocations()
{
	return RequiredBufferLocations;
}
TArray<FVector> UBaseGenerations::GetUsableLocations()
{
	return UsableLocations;
}
TArray<FVector> UBaseGenerations::GetTotalUsedLocations()
{
	return TotalUsedLocations;
}

TArray<FVector> UBaseGenerations::CreateRequiredLocations(FVector original)
{
	TArray<FVector> locs;
	for (int i = 0; i < TemplatedRequiredLocations.Num(); i++)
		locs.Emplace(original + TemplatedRequiredLocations[i]);

	return locs;
}

// void UBaseGenerations::AddToUsedSpots(TArray<FVector> locs) {
//	for (int i = 0; i < locs.Num(); i++)
//		AllUsedSpots.Emplace(locs[i]);
// }
// TArray<FVector> UBaseGenerations::GetUsedSpots() {
//	return AllUsedSpots;
// }

TArray<FVector> UBaseGenerations::GetUnusedSpotsLeft()
{
	TArray<FVector> result;
	for (int i = 0; i < UsableLocations.Num(); i++)
	{
		if (!IsWithinRequiredSpots(UsableLocations[i]) || !IsWithinRequiredBufferSpots(UsableLocations[i]))
			result.Emplace(UsableLocations[i]);
	}
	return result;
}

TArray<FVector> UBaseGenerations::CombineList(TArray<FVector> locs1, TArray<FVector> locs2)
{
	for (int i = 0; i < locs1.Num(); i++)
	{
		locs2.Emplace(locs1[i]);
	}
	return locs2;
}

UBaseGenerations *UBaseGenerations::SetStartAndEndZones(TArray<FVector> startZones, TArray<FVector> endZones)
{
	StartZones = startZones;
	EndZones = endZones;
	return this;
}

// bool UBaseGenerations::IsWithinAllowedList(FVector location) {
//	for (int i = 0; i < AllowedSpots.Num(); i++)
//		if (AllowedSpots[i] == location)
//			return true;
//
//	return false;
// }
//
// bool UBaseGenerations::IsWithinCurrentUsedList(FVector location) {
//	for (int i = 0; i < AllUsedSpots.Num(); i++)
//		if (AllUsedSpots[i] == location)
//			return true;
//
//	return false;
// }

bool UBaseGenerations::IsRequiredSpotBottomLeft(FVector loc)
{
	auto left = loc + FVector(-100, 0, 0);
	auto down = loc + FVector(0, -100, 0);

	for (int i = 0; i < RequiredLocations.Num(); i++)
		if (RequiredLocations[i] == left || RequiredLocations[i] == down)
			return false;

	return true;
}
bool UBaseGenerations::IsRequiredSpotTopLeft(FVector loc)
{
	auto right = loc + FVector(100, 0, 0);
	auto down = loc + FVector(0, -100, 0);

	for (int i = 0; i < RequiredLocations.Num(); i++)
	{
		if (RequiredLocations[i] == right || RequiredLocations[i] == down)
			return false;
	}
	return true;
}
bool UBaseGenerations::IsRequiredSpotBottomRight(FVector loc)
{
	auto left = loc + FVector(-100, 0, 0);
	auto up = loc + FVector(0, 100, 0);

	for (int i = 0; i < RequiredLocations.Num(); i++)
	{
		if (RequiredLocations[i] == left || RequiredLocations[i] == up)
			return false;
	}

	return true;
}
bool UBaseGenerations::IsRequiredSpotTopRight(FVector loc)
{
	auto right = loc + FVector(100, 0, 0);
	auto up = loc + FVector(0, 100, 0);

	for (int i = 0; i < RequiredLocations.Num(); i++)
	{
		if (RequiredLocations[i] == right || RequiredLocations[i] == up)
			return false;
	}
	return true;
}

bool UBaseGenerations::IsRequiredSpotLeftWall(FVector loc)
{
	auto left = loc + FVector(0, -100, 0);
	return !IsWithinRequiredSpots(left);
}
bool UBaseGenerations::IsRequiredSpotRightWall(FVector loc)
{
	auto right = loc + FVector(0, 100, 0);
	return !IsWithinRequiredSpots(right);
}
bool UBaseGenerations::IsRequiredSpotTopWall(FVector loc)
{
	auto up = loc + FVector(100, 0, 0);
	return !IsWithinRequiredSpots(up);
}
bool UBaseGenerations::IsRequiredSpotBottomWall(FVector loc)
{
	auto down = loc + FVector(-100, 0, 0);
	return !IsWithinRequiredSpots(down);
}

bool UBaseGenerations::IsWithinRequiredSpots(FVector location)
{
	for (int i = 0; i < RequiredLocations.Num(); i++)
		if (RequiredLocations[i] == location)
			return true;

	return false;
}
bool UBaseGenerations::IsWithinRequiredBufferSpots(FVector location)
{
	for (int i = 0; i < RequiredBufferLocations.Num(); i++)
		if (RequiredBufferLocations[i] == location)
			return true;

	return false;
}

TArray<FVector> UBaseGenerations::GenerateBufferAroundLocations(TArray<FVector> locations, int buffer)
{
	TArray<FVector> result;

	for (int i = 0; i < locations.Num(); i++)
	{

		TArray<FVector> adjacents = GetAdjacentsAroundLocation(locations[i], buffer);

		for (int x = 0; x < adjacents.Num(); x++)
		{
			if (IsWithinList(locations, adjacents[x]))
				continue;

			result.Emplace(adjacents[x]);
		}
	}

	return result;
}
#pragma optimize("", off)
TArray<FVector> UBaseGenerations::GetAdjacentsAroundLocation(FVector loc, int adjacentRadius)
{
	TArray<FVector> locs;
	for (int i = 1; i <= adjacentRadius; i++)
	{
		for (int x = -i; x <= i; x++)
		{
			// locs.Emplace(loc + FVector((100 * x), 0 + 0, 0));
			for (int y = -i; y <= i; y++)
			{
				if (x == 0 && y == 0)
					continue;
				locs.Emplace(loc + FVector((100 * x), 0 + (100 * y), 0));
				// locs.Emplace(loc + FVector(0, 0 + (100 * y), 0));
			}
		}
		// radius around

		// locs.Emplace(loc + FVector((100 * i), 0 + (100 * i), 0));
		// locs.Emplace(loc + FVector((-100 * i), 0 + (-100 * i), 0));

		// locs.Emplace(loc + FVector(0, 0 + (100 * i), 0));
		// locs.Emplace(loc + FVector((100 * i), 0, 0));

		// locs.Emplace(loc + FVector(0, (-100 * i), 0));
		// locs.Emplace(loc + FVector(0 + (-100 * i), 0, 0));
	}

	return locs;
}
#pragma optimize("", on)
FVector UBaseGenerations::GetRandomLocationInList(TArray<FVector> locs)
{
	if (locs.IsEmpty())
		return FVector::ZeroVector;

	return locs[RandomStream.RandRange(0, locs.Num() - 1)];
}

void UBaseGenerations::SpawnDebugGrid_SetIsStart(FVector location, float delay)
{
	auto world = GetWorld();
	FPlatformProcess::Sleep(delay);
	location += FVector(-50, -50, 0);
	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, location]
																			  {
		auto actor = world->SpawnActor<AWorldGridItemActor>(location, FRotator::ZeroRotator);
		actor->SetIsStart(); }, TStatId(), nullptr, ENamedThreads::GameThread);
}
void UBaseGenerations::SpawnDebugGrid_SetIsEnd(FVector location, float delay)
{
	auto world = GetWorld();
	FPlatformProcess::Sleep(delay);
	location += FVector(-50, -50, 0);
	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, location]
																			  {
		auto actor = world->SpawnActor<AWorldGridItemActor>(location, FRotator::ZeroRotator);
		actor->SetIsEnd(); }, TStatId(), nullptr, ENamedThreads::GameThread);
}
void UBaseGenerations::SpawnDebugGrid_SetIsOffshoot(FVector location, float delay)
{
	auto world = GetWorld();
	FPlatformProcess::Sleep(delay);
	location += FVector(-50, -50, 0);
	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, location]
																			  {
		auto actor = world->SpawnActor<AWorldGridItemActor>(location, FRotator::ZeroRotator);
		actor->SetIsOffshoot(); }, TStatId(), nullptr, ENamedThreads::GameThread);
}
void UBaseGenerations::SpawnDebugGrid_SetIsSpawn(FVector location, float delay)
{
	auto world = GetWorld();
	FPlatformProcess::Sleep(delay);
	location += FVector(-50, -50, 0);
	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, location]
																			  {
		auto actor = world->SpawnActor<AWorldGridItemActor>(location, FRotator::ZeroRotator);
		actor->SetIsSpawn(); }, TStatId(), nullptr, ENamedThreads::GameThread);
}
void UBaseGenerations::SpawnDebugGrid_SetIsObstacle(FVector location, float delay)
{
	auto world = GetWorld();
	FPlatformProcess::Sleep(delay);
	location += FVector(-50, -50, 0);
	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, location]
																			  {
		auto actor = world->SpawnActor<AWorldGridItemActor>(location, FRotator::ZeroRotator);
		actor->SetIsObstacle(); }, TStatId(), nullptr, ENamedThreads::GameThread);
}

void UBaseGenerations::SpawnDebugGrid_SetIsStart(TArray<FVector> locations, FVector offset, float delay)
{
	for (int i = 0; i < locations.Num(); i++)
		SpawnDebugGrid_SetIsStart(locations[i] + offset, delay);
}
void UBaseGenerations::SpawnDebugGrid_SetIsEnd(TArray<FVector> locations, FVector offset, float delay)
{
	for (int i = 0; i < locations.Num(); i++)
		SpawnDebugGrid_SetIsEnd(locations[i] + offset, delay);
}
void UBaseGenerations::SpawnDebugGrid_SetIsOffshoot(TArray<FVector> locations, FVector offset, float delay)
{
	for (int i = 0; i < locations.Num(); i++)
		SpawnDebugGrid_SetIsOffshoot(locations[i] + offset, delay);
}
void UBaseGenerations::SpawnDebugGrid_SetIsSpawn(TArray<FVector> locations, FVector offset, float delay)
{
	for (int i = 0; i < locations.Num(); i++)
		SpawnDebugGrid_SetIsSpawn(locations[i] + offset, delay);
}
void UBaseGenerations::SpawnDebugGrid_SetIsObstacle(TArray<FVector> locations, FVector offset, float delay)
{
	for (int i = 0; i < locations.Num(); i++)
		SpawnDebugGrid_SetIsObstacle(locations[i] + offset, delay);
}

bool UBaseGenerations::IsWithinList(TArray<FVector> list, FVector location)
{
	for (int i = 0; i < list.Num(); i++)
		if (list[i] == location)
			return true;

	return false;
}

TArray<FVector> UBaseGenerations::RemoveListFromList(TArray<FVector> bigList, TArray<FVector> removesThisListFromItemsFromBigList)
{
	for (int i = 0; i < removesThisListFromItemsFromBigList.Num(); i++)
		for (int x = 0; x < bigList.Num(); x++)
			if (removesThisListFromItemsFromBigList[i] == bigList[x])
			{
				bigList.RemoveAt(x);
				break;
			}

	return bigList;
}
#pragma optimize("", off)
UObject *UBaseGenerations::LoadBlueprintActorInThread(FString reference, UObject *&obj, int secondsToWaitUntilGiveup)
{
	TGraphTask<LoadBpActorTask>::CreateTask().ConstructAndDispatchWhenReady(reference, &obj);
	int iteration = 0;
	while (!obj)
	{
		FPlatformProcess::Sleep(1);
		secondsToWaitUntilGiveup++;

		if (iteration >= secondsToWaitUntilGiveup)
		{
			UDebugMessages::LogError(this, "failed to get blueprint actor, after " + FString::SanitizeFloat(secondsToWaitUntilGiveup) + " seconds");
			return nullptr;
		}
	}

	return obj;
}
#pragma optimize("", on)