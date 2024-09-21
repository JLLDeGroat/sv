// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadGenerations.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Environment/Natural/NaturalRoad.h"



URoadGenerations::URoadGenerations() {
	MaxTotalIteraions = 3000;
}

UBaseGenerations* URoadGenerations::Generate() {
	TotalIterations = 0;
	if (!Route.IsEmpty()) {
		//route set by parent object
		//simple route place
		for (int i = 0; i < Route.Num(); i++) {
			PlaceRoad(Route[i]);
		}
		return this;
	}

	RoadAllowedList = UsableLocations;//need to also add the spawn and endzones here
	for (int i = 0; i < StartZones.Num(); i++) RoadAllowedList.Emplace(StartZones[i]);
	for (int i = 0; i < EndZones.Num(); i++) RoadAllowedList.Emplace(EndZones[i]);

	TArray<FVector> route;
	route.Emplace(StartLocation);

	PlaceRoad(StartLocation);
	PlaceRoad(EndLocation);

	FindPrimaryRouteBetweenRecursive(route, EndLocation, 70);

	if (Route.IsEmpty()) {
		UDebugMessages::LogError(this, "Generation failed");
	}

	for (int i = 0; i < Route.Num(); i++) {
		PlaceRoad(Route[i]);
	}

	return this;
}

TArray<FVector> URoadGenerations::GetRoadRoute() {
	return Route;
}
URoadGenerations* URoadGenerations::SetRoadRoute(TArray<FVector> locations) {
	Route = locations;
	return this;
}

URoadGenerations* URoadGenerations::SetRoadMaxIterations(int amount) {
	MaxTotalIteraions = amount;
	return this;
}

void URoadGenerations::PlaceRoad(FVector loc) {
	auto finalLoc = loc + FVector(50, 50, 0);
	auto world = GetWorld();
	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, loc] {
		auto actor = world->SpawnActor<ANaturalRoad>(loc, FRotator(0));
		}, TStatId(), nullptr, ENamedThreads::GameThread);
}

void URoadGenerations::FindPrimaryRouteBetweenRecursive(TArray<FVector> currentRoute, FVector end, int maxRoute) {
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

		if (!Route.IsEmpty())
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
			Route = finalRoute;
			return;
		}

		auto newRoute = currentRoute;
		newRoute.Emplace(adjacentGridItems[i]);

		//SpawnDebugGrid_SetIsObstacle(adjacentGridItems[i], .001f);

		auto world = GetWorld();
		auto owner = this;
		FindPrimaryRouteBetweenRecursive(newRoute, end, maxRoute);
	}
}

bool URoadGenerations::IsWithinList(TArray<FVector> locations, FVector loc) {
	for (int i = 0; i < locations.Num(); i++)
		if (locations[i] == loc)
			return true;

	return false;
}
bool URoadGenerations::CanReachDestination(FVector location, FVector end, int steps) {
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
bool URoadGenerations::IsFurtherFromEnd(FVector last, FVector current, FVector end) {
	auto distFromLast = FVector::Dist(last, end);
	auto distFromNew = FVector::Dist(current, end);

	auto difference = distFromNew - distFromLast;
	//UDebugMessages::LogDisplay(this, "diff " + FString::SanitizeFloat(difference, 2));
	return difference > 75;
}

TArray<FVector> URoadGenerations::GetAdjacentGridItems(FVector item) {

	auto left = FVector(item.X - 100, item.Y, 0);
	auto right = FVector(item.X + 100, item.Y, 0);
	auto up = FVector(item.X, item.Y - 100, 0);
	auto down = FVector(item.X, item.Y + 100, 0);

	TArray<FVector> result;

	if (IsWithinList(RoadAllowedList, left)) result.Emplace(left);
	if (IsWithinList(RoadAllowedList, right)) result.Emplace(right);
	if (IsWithinList(RoadAllowedList, up)) result.Emplace(up);
	if (IsWithinList(RoadAllowedList, down)) result.Emplace(down);

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
