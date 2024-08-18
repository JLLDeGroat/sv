// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingInteriorGeneration.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Environment/Constructions/Interior/ComputerSetup.h"
#include "../../Environment/Constructions/Interior/CornerTable.h"
#include "../../Environment/Constructions/Interior/LongTable.h"
#include "../../Environment/Constructions/Interior/RollingTable.h"

UBuildingInteriorGeneration::UBuildingInteriorGeneration() {
	InternalRouteAmounts = 1;
}

UBuildingInteriorGeneration* UBuildingInteriorGeneration::SetDoorLocations(TArray<FVector> locations) {
	DoorLocations = locations;
	return this;
}
UBuildingInteriorGeneration* UBuildingInteriorGeneration::SetDoorLocation(FVector location) {
	DoorLocations.Emplace(location);
	return this;
}
UBuildingInteriorGeneration* UBuildingInteriorGeneration::SetInternalRouteAmount(int amount) {
	InternalRouteAmounts = amount;
	return this;
}
UBaseGenerations* UBuildingInteriorGeneration::Generate() {
	TotalIterations = 0;
	MaxTotalIteraions = 300;

	if (DoorLocations.Num() == 0) {
		UDebugMessages::LogError(this, "No Doors! ERROR");
		return this;
	}

	TArray<FVector> doorConnects;
	if (DoorLocations.Num() > 1) {
		for (int i = 0; i < DoorLocations.Num(); i++) {
			if (i == 0)
				continue;

			FVector thisDoor = DoorLocations[i];
			FVector previousDoor = DoorLocations[i - 1];

			TArray<FVector> arr;
			arr.Emplace(previousDoor);
			FindPrimaryRouteBetweenRecursive(arr, thisDoor, 20);

			if (!InteriorRoute.IsEmpty()) {
				doorConnects = CombineList(doorConnects, InteriorRoute);
				InteriorRoute.Empty();
			}
		}
	}

	ValidInteriorLocations = RemoveListFromList(UsableLocations, doorConnects);

	auto cornerLocations = DetermineCorners();
	for (int i = 0; i < cornerLocations.Num(); i++) {
		if (!IsWithinList(DoorLocations, cornerLocations[i])) {
			auto random = RandomStream.RandRange(1, 1001);
			auto shouldRotate = random > 500;

			TArray<FVector>requiredLocations;
			auto templatedCorner = GetCornerTableTemplatedLocations(shouldRotate);
			for (int j = 0; j < templatedCorner.Num(); j++) {
				requiredLocations.Emplace(cornerLocations[i] + templatedCorner[j]);
			}

			bool bIsValid = true;
			for (int j = 0; j < requiredLocations.Num(); j++) {
				if (!IsWithinList(ValidInteriorLocations, requiredLocations[j]))
					bIsValid = false;
			}

			if (bIsValid) {
				ValidInteriorLocations = RemoveListFromList(ValidInteriorLocations, requiredLocations);
				auto world = GetWorld();
				auto finalSpawnLoc = cornerLocations[i] + FVector(-50, -50, 0) + (shouldRotate ? FVector(100, 0, 0) : FVector(0, 100, 0));
				FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, finalSpawnLoc, shouldRotate] {
					FActorSpawnParameters param;
					auto actor = world->SpawnActor<ACornerTable>(finalSpawnLoc, shouldRotate ? FRotator(0, 180, 0) : FRotator::ZeroRotator, param);
					}, TStatId(), nullptr, ENamedThreads::GameThread);
			}
		}
	}

	TArray<FVector> routedLocations;
	for (int i = 0; i < InternalRouteAmounts; i++) {
		routedLocations = CombineList(GenerateInternalRoute(), routedLocations);
	}

	auto finalRoute = CombineList(routedLocations, doorConnects);
	ValidInteriorLocations = RemoveListFromList(ValidInteriorLocations, finalRoute);

	//SpawnDebugGrid_SetIsEnd(ValidInteriorLocations);

	int iterationsFailed = 0;
	while (iterationsFailed < 10 && !ValidInteriorLocations.IsEmpty()) {
		if (!GenerateItemInBuilding(GetRandomLocationInList(ValidInteriorLocations))) {
			iterationsFailed += 1;
		}
	}
	return this;
}
TArray<FVector> UBuildingInteriorGeneration::GenerateInternalRoute() {
	auto randomLocations = GetRandomLocationInList(UsableLocations);
	TArray<FVector> routedLocations;
	if (!IsWithinList(DoorLocations, randomLocations)) {
		TArray<FVector> arr;
		arr.Emplace(randomLocations);
		FindPrimaryRouteBetweenRecursive(arr, GetRandomLocationInList(DoorLocations), 30);
		if (!InteriorRoute.IsEmpty()) {
			routedLocations = CombineList(routedLocations, InteriorRoute);
			InteriorRoute.Empty();
		}
	}
	return routedLocations;
}
bool UBuildingInteriorGeneration::GenerateItemInBuilding(FVector desiredLocation) {
	auto random = RandomStream.RandRange(1, 1001);
	auto randomRotation = RandomStream.RandRange(1, 1001);
	auto bShouldRotate = randomRotation > 500;
	UClass* classToSpawn = nullptr;
	TArray<FVector> templatedLocations;
	TArray<FVector> requiredLocations;
	if (random > 550) {
		classToSpawn = ALongTable::StaticClass();
		templatedLocations = GetLongTableTemplatedLocations(bShouldRotate);
	}
	else {
		classToSpawn = ARollingTable::StaticClass();
		templatedLocations = GetRollingTableTemplatedLocations(bShouldRotate);
	}

	for (int i = 0; i < templatedLocations.Num(); i++) {
		requiredLocations.Emplace(desiredLocation + templatedLocations[i]);
	}

	bool bIsValid = true;
	for (int i = 0; i < requiredLocations.Num(); i++) {
		if (!IsWithinList(ValidInteriorLocations, requiredLocations[i])) {
			bIsValid = false;
			break;
		}
	}

	if (bIsValid) {
		ValidInteriorLocations = RemoveListFromList(ValidInteriorLocations, requiredLocations);

		auto world = GetWorld();
		auto finalSpawnLoc = desiredLocation + FVector(-50, -50, 0);
		FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([classToSpawn, world, finalSpawnLoc, bShouldRotate] {
			FActorSpawnParameters param;
			auto actor = world->SpawnActor<AEnvironmentActor>(classToSpawn, finalSpawnLoc, bShouldRotate ? FRotator(0, 90, 0) : FRotator::ZeroRotator, param);
			}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
	return bIsValid;
}

TArray<FVector> UBuildingInteriorGeneration::GetLongTableTemplatedLocations(bool bShouldRotate) {
	TArray<FVector> templatedLocations;

	if (bShouldRotate) {
		templatedLocations.Emplace(FVector(-100, 0, 0));
		templatedLocations.Emplace(FVector(-100, 100, 0));
		templatedLocations.Emplace(FVector(-100, 200, 0));
	}
	else {
		templatedLocations.Emplace(FVector(0, 0, 0));
		templatedLocations.Emplace(FVector(100, 0, 0));
		templatedLocations.Emplace(FVector(200, 0, 0));
	}
	return templatedLocations;
}
TArray<FVector> UBuildingInteriorGeneration::GetRollingTableTemplatedLocations(bool bShouldRotate) {
	TArray<FVector> templatedLocations;
	if (bShouldRotate) {
		templatedLocations.Emplace(FVector(-100, 0, 0));
	}
	else {
		templatedLocations.Emplace(FVector(0, 0, 0));
	}
	return templatedLocations;
}
TArray<FVector> UBuildingInteriorGeneration::GetCornerTableTemplatedLocations(bool bShouldRotate) {
	TArray<FVector> templatedLocations;
	if (bShouldRotate) {
		templatedLocations.Emplace(FVector(0, 0, 0));
		templatedLocations.Emplace(FVector(0, 100, 0));
		templatedLocations.Emplace(FVector(0, 200, 0));
		templatedLocations.Emplace(FVector(-100, 0, 0));
	}
	else {
		templatedLocations.Emplace(FVector(0, 0, 0));
		templatedLocations.Emplace(FVector(100, 0, 0));
		templatedLocations.Emplace(FVector(0, -100, 0));
		templatedLocations.Emplace(FVector(0, -200, 0));
	}
	return templatedLocations;
}

bool UBuildingInteriorGeneration::GenerateLongTable(FVector desiredLocation) {
	TArray<FVector> templatedLocations;
	templatedLocations.Emplace(FVector(0, 0, 0));
	templatedLocations.Emplace(FVector(100, 0, 0));
	templatedLocations.Emplace(FVector(200, 0, 0));

	TArray<FVector> requiredLocations;
	for (int i = 0; i < templatedLocations.Num(); i++) {
		requiredLocations.Emplace(desiredLocation + templatedLocations[i]);
	}

	bool bIsValid = true;
	for (int i = 0; i < requiredLocations.Num(); i++) {
		if (!IsWithinList(ValidInteriorLocations, requiredLocations[i]))
			bIsValid = false;
	}

	if (bIsValid) {
		auto world = GetWorld();
		auto finalSpawnLoc = desiredLocation + FVector(-50, -50, 0);
		FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, finalSpawnLoc] {
			auto actor = world->SpawnActor<ALongTable>(finalSpawnLoc, FRotator::ZeroRotator);
			}, TStatId(), nullptr, ENamedThreads::GameThread);
	}

	//ValidInteriorLocations = RemoveListFromList(ValidInteriorLocations, requiredLocations);
	return bIsValid;
}

TArray<FVector> UBuildingInteriorGeneration::DetermineCorners() {
	TArray<FVector> corners;
	if (UsableLocations.Num() == 0)
	{
		// If there are no points, return an empty array of corners
		return corners;
	}

	FVector MinPoint = UsableLocations[0];
	FVector MaxPoint = UsableLocations[0];

	// Iterate over each point to determine the min and max extents
	for (const FVector& Point : UsableLocations)
	{
		MinPoint.X = FMath::Min(MinPoint.X, Point.X);
		MinPoint.Y = FMath::Min(MinPoint.Y, Point.Y);
		MinPoint.Z = FMath::Min(MinPoint.Z, Point.Z);

		MaxPoint.X = FMath::Max(MaxPoint.X, Point.X);
		MaxPoint.Y = FMath::Max(MaxPoint.Y, Point.Y);
		MaxPoint.Z = FMath::Max(MaxPoint.Z, Point.Z);
	}

	// Generate the four corners of the bounding box
	corners.Add(FVector(MinPoint.X, MinPoint.Y, MinPoint.Z)); // Bottom-left corner
	corners.Add(FVector(MinPoint.X, MaxPoint.Y, MinPoint.Z)); // Bottom-right corner
	corners.Add(FVector(MaxPoint.X, MinPoint.Y, MinPoint.Z)); // Top-left corner
	corners.Add(FVector(MaxPoint.X, MaxPoint.Y, MinPoint.Z)); // Top-right corner
	return corners;
}


void UBuildingInteriorGeneration::FindPrimaryRouteBetweenRecursive(TArray<FVector> currentRoute, FVector end, int maxRoute) {
	if (currentRoute.Num() > maxRoute)
		return;

	TotalIterations += 1;
	if (TotalIterations > MaxTotalIteraions) {
		UDebugMessages::LogError(this, "reached max iterations, exiting");
		return;
	}

	auto lastRoute = currentRoute[currentRoute.Num() - 1];
	auto adjacentGridItems = GetAdjacentGridItems(lastRoute);

	for (int i = 0; i < adjacentGridItems.Num(); i++) {

		if (adjacentGridItems[i] == end) {
			UDebugMessages::LogError(this, "found end");
		}

		if (!InteriorRoute.IsEmpty())
			return;

		if (IsWithinList(currentRoute, adjacentGridItems[i]))
			continue;

		if (!CanReachDestination(adjacentGridItems[i], end, maxRoute - currentRoute.Num()))
			continue;

		if (IsFurtherFromEnd(lastRoute, adjacentGridItems[i], end))
			continue;

		if (adjacentGridItems[i] == end) {
			TArray<FVector> finalRoute;
			for (int x = 0; x < currentRoute.Num(); x++)
				finalRoute.Emplace(currentRoute[x]);

			finalRoute.Emplace(adjacentGridItems[i]);
			InteriorRoute = finalRoute;
			return;
		}

		auto newRoute = currentRoute;
		newRoute.Emplace(adjacentGridItems[i]);

		auto world = GetWorld();
		auto owner = this;
		FindPrimaryRouteBetweenRecursive(newRoute, end, maxRoute);
	}
}
TArray<FVector> UBuildingInteriorGeneration::GetAdjacentGridItems(FVector item) {

	auto left = FVector(item.X - 100, item.Y, 0);
	auto right = FVector(item.X + 100, item.Y, 0);
	auto up = FVector(item.X, item.Y - 100, 0);
	auto down = FVector(item.X, item.Y + 100, 0);

	TArray<FVector> result;

	if (IsWithinList(UsableLocations, left)) result.Emplace(left);
	if (IsWithinList(UsableLocations, right)) result.Emplace(right);
	if (IsWithinList(UsableLocations, up)) result.Emplace(up);
	if (IsWithinList(UsableLocations, down)) result.Emplace(down);

	if (result.Num() > 0)
	{
		int32 LastIndex = result.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = RandomStream.RandRange(i, LastIndex);
			if (i != Index)
				result.Swap(i, Index);
		}
	}
	return result;
}
bool UBuildingInteriorGeneration::CanReachDestination(FVector location, FVector end, int steps) {
	auto stepBuffer = 3;

	auto xmovement = location.X - end.X;
	if (xmovement < 0) xmovement *= -1;

	auto ymovement = location.Y - end.Y;
	if (ymovement < 0) ymovement *= -1;

	if ((ymovement + xmovement) / 100 <= (steps - stepBuffer)) {
		return true;
	}
	else return false;
}
bool UBuildingInteriorGeneration::IsFurtherFromEnd(FVector last, FVector current, FVector end) {
	auto distFromLast = FVector::Dist(last, end);
	auto distFromNew = FVector::Dist(current, end);

	auto difference = distFromNew - distFromLast;
	//UDebugMessages::LogDisplay(this, "diff " + FString::SanitizeFloat(difference, 2));
	return difference > 75;
}