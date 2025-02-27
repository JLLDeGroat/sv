// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseLevelIndex.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../../GameModes/GameplayMode.h"
#include "../../../../Instance/Managers/LevelGenerationManager.h"
#include "../../../../GameModes/Managers/LevelSpawnerManager.h"
#include "../../../../Environment/Natural/RockSection.h"
#include "../../../../World/WorldGridItemActor.h"
#include "../../../../Environment/Fog/FogManager.h"

UBaseLevelIndex::UBaseLevelIndex()
{
	RandomStream = FRandomStream();
	bIsAlive = true;
}

UBaseLevelIndex *UBaseLevelIndex::Setup(UWorld *world, FRandomStream randomStream)
{
	World = world;
	RandomStream = randomStream;
	return this;
}
UWorld *UBaseLevelIndex::GetWorld() const
{
	return World;
}
void UBaseLevelIndex::KillObject()
{
	bIsAlive = false;
}

void UBaseLevelIndex::GenerateLevel()
{
	UDebugMessages::LogError(this, "Cannot generate level, no override for method GenerateLevel");
}

void UBaseLevelIndex::AddDebugActor(AActor *actor)
{
	DebugActors.Emplace(actor);
}

void UBaseLevelIndex::CreateGrid(int maxX, int maxY, int elevation)
{
	for (int j = 0; j <= elevation; j++)
	{
		for (int i = 0; i <= maxX; i++)
		{
			for (int x = 0; x <= maxY; x++)
			{
				Grid.Emplace(FVector(i * 100, x * 100, j * 200));
				GridData.AddDataItem(i, x, j);

				if (j == 0)
					TwoDGrid.Emplace(FVector2D(i, x));
			}
		}
	}

	AFogManager *fogManager = nullptr;
	auto &actors = GetWorld()->GetCurrentLevel()->Actors;
	for (int i = 0; i < actors.Num(); i++)
	{
		if (actors[i])
		{
			if (AFogManager *fManager = Cast<AFogManager>(actors[i]))
			{
				fogManager = fManager;
				break;
			}
		}
	}

	if (fogManager)
	{
		auto gridItems = GridData.GetGridItems();
		for (FGridDataItem dataItem : GridData.GetGridItems())
		{
			auto gridLoc = dataItem.GetLocation();
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady(
				[fogManager, gridLoc]
				{
					fogManager->AddComponentAtLocation(gridLoc);
				},
				TStatId(), nullptr, ENamedThreads::GameThread);
		}
		// for (int i = 0; i < gridItems.Num(); i++)
		// {
		// 	auto gridLoc = gridItems[i].GetLocation();
		// 	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady(
		// 		[fogManager, gridLoc]
		// 		{
		// 			fogManager->AddComponentAtLocation(gridLoc);
		// 		},
		// 		TStatId(), nullptr, ENamedThreads::GameThread);
		// }
	}
}
void UBaseLevelIndex::SetSpawnAndEndZone()
{
	int random = RandomStream.RandRange(1, 1001);

	auto spawnPosStartX = (MaxX / 2) - 3;
	auto spawnPosEndX = (MaxX / 2) + 3;
	auto spawnPosStartY = 1;
	auto spawnPosEndY = 2;

	auto endPosStartX = (MaxX / 2) - 3;
	auto endPosEndX = (MaxX / 2) + 3;
	auto endPosStartY = MaxY - 1;
	auto endPosEndY = MaxY;

	if (random < 500)
	{
		spawnPosStartY = MaxY - 1;
		spawnPosEndY = MaxY;

		endPosStartY = 1;
		endPosEndY = 2;
	}
	else if (random < 750)
	{
		spawnPosStartX = 1;
		spawnPosEndX = 2;

		spawnPosStartY = (MaxY / 2) - 3;
		spawnPosEndY = (MaxY / 2) + 3;

		endPosStartY = (MaxY / 2) - 3;
		endPosEndY = (MaxY / 2) + 3;
		endPosStartX = MaxX - 1;
		endPosEndX = MaxX;
	}
	else
	{
		spawnPosStartX = MaxX - 1;
		spawnPosEndX = MaxX;

		spawnPosStartY = (MaxY / 2) - 3;
		spawnPosEndY = (MaxY / 2) + 3;

		endPosStartY = (MaxY / 2) - 3;
		endPosEndY = (MaxY / 2) + 3;

		endPosEndX = 2;
		endPosStartX = 1;
	}

	for (int x = spawnPosStartX; x <= spawnPosEndX; x++)
	{
		for (int y = spawnPosStartY; y <= spawnPosEndY; y++)
		{
			SpawnZone.Emplace(FVector(x * 100, y * 100, 0));
		}
	}

	for (int x = endPosStartX; x <= endPosEndX; x++)
	{
		for (int y = endPosStartY; y <= endPosEndY; y++)
		{
			EndZone.Emplace(FVector(x * 100, y * 100, 0));
		}
	}

	SetLocationsAsSpawnZone(SpawnZone);
	SetLocationsAsEndZone(EndZone);
}
void UBaseLevelIndex::GenerateBoundaryWalls()
{
	auto gameMode = GetWorld()->GetAuthGameMode<AGameplayMode>();
	auto spawnerManager = gameMode->GetLevelSpawnerManager();

	auto gridItems = GridData.GetGridItems();

	for (int i = 0; i < gridItems.Num(); i++)
	{
		auto thisLocation = gridItems[i].GetLocation();
		// bottom left corner
		if (gridItems[i].GetLocation().X == 0 && gridItems[i].GetLocation().Y == 0)
		{
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady(
				[spawnerManager, thisLocation]
				{
					spawnerManager->SpawnMajorWallAt(thisLocation, FRotator(0, 90, 0));
				},
				TStatId(), nullptr, ENamedThreads::GameThread);
		}
		// bottom right corner
		else if (gridItems[i].GetLocation().Y == MaxY * 100 && gridItems[i].GetLocation().X == 0)
		{
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([spawnerManager, thisLocation]
																				 {
				spawnerManager->SpawnMajorWallAt(thisLocation, FRotator(0, 0, 0));
				spawnerManager->SpawnMajorWallAt(thisLocation + FVector(0, 100, 0), FRotator(0, 0, 0));
				spawnerManager->SpawnMajorWallAt(thisLocation + FVector(0, 100, 0), FRotator(0, 90, 0)); }, TStatId(), nullptr, ENamedThreads::GameThread);
		}
		// top left corner
		else if (gridItems[i].GetLocation().X == MaxX * 100 && gridItems[i].GetLocation().Y == 0)
		{
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady(
				[spawnerManager, thisLocation]
				{ spawnerManager->SpawnMajorWallAt(thisLocation, FRotator(0, 90, 0)); }, TStatId(), nullptr, ENamedThreads::GameThread);
		}
		// top right corner
		else if (gridItems[i].GetLocation().X == MaxX * 100 && gridItems[i].GetLocation().Y == MaxY * 100)
		{
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady(
				[spawnerManager, thisLocation]
				{
					// spawnerManager->SpawnMajorWallAt(thisLocation + FVector(0, 100, 0), FRotator(0, 90, 0));
					spawnerManager->SpawnMajorWallAt(thisLocation + FVector(100, 0, 0), FRotator(0, 0, 0));
					spawnerManager->SpawnMajorWallAt(thisLocation + FVector(100, 100, 0), FRotator(0, 0, 0));

					spawnerManager->SpawnMajorWallAt(thisLocation + FVector(0, 100, 0), FRotator(0, 90, 0));
				},
				TStatId(), nullptr, ENamedThreads::GameThread);
		}
		else if (gridItems[i].GetLocation().X == 0)
		{
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady(
				[spawnerManager, thisLocation]
				{
					spawnerManager->SpawnMajorWallAt(thisLocation);
				},
				TStatId(), nullptr, ENamedThreads::GameThread);
		}
		else if (gridItems[i].GetLocation().X == MaxX * 100)
		{
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady(
				[spawnerManager, thisLocation]
				{
					spawnerManager->SpawnMajorWallAt(thisLocation + FVector(100, 0, 0));
				},
				TStatId(), nullptr, ENamedThreads::GameThread);
		}
		else if (gridItems[i].GetLocation().Y == 0)
		{
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady(
				[spawnerManager, thisLocation]
				{
					spawnerManager->SpawnMajorWallAt(thisLocation, FRotator(0, 90, 0));
				},
				TStatId(), nullptr, ENamedThreads::GameThread);
		}
		else if (gridItems[i].GetLocation().Y == MaxY * 100)
		{
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady(
				[spawnerManager, thisLocation]
				{
					spawnerManager->SpawnMajorWallAt(thisLocation + FVector(0, 100, 0), FRotator(0, 90, 0));
				},
				TStatId(), nullptr, ENamedThreads::GameThread);
		}
	}
}

void UBaseLevelIndex::FindPrimaryRouteBetweenRecursive(TArray<FVector> currentRoute, FVector end, int maxRoute, int routeId, bool bDebug)
{
	if (!bIsAlive)
	{
		UDebugMessages::LogError(this, "thread killed ending here");
		return;
	}

	if (currentRoute.Num() > maxRoute)
		return;

	if (currentRoute.Num() == 0)
		return UDebugMessages::LogError(this, "tried to find route with no start");

	auto lastRoute = currentRoute[currentRoute.Num() - 1];
	auto adjacentGridItems = GetAdjacentGridItems(lastRoute);

	for (int i = 0; i < adjacentGridItems.Num(); i++)
	{
		if (!GetRouteById(routeId).IsEmpty())
			return;

		if (IsWithinList(currentRoute, adjacentGridItems[i]))
			continue;

		if (!CanReachDestination(adjacentGridItems[i], end, maxRoute - currentRoute.Num()))
			continue;

		if (IsWithinList(FindRouteIgnoreLocations, adjacentGridItems[i]))
			continue;

		if (IsFurtherFromEnd(lastRoute, adjacentGridItems[i], end))
			continue;

		if (adjacentGridItems[i] == end)
		{
			TArray<FVector> finalRoute;
			for (int x = 0; x < currentRoute.Num(); x++)
				finalRoute.Emplace(currentRoute[x]);

			finalRoute.Emplace(adjacentGridItems[i]);
			SetRouteById(routeId, finalRoute);
			return;
		}

		auto newRoute = currentRoute;
		newRoute.Emplace(adjacentGridItems[i]);

		if (bDebug)
			SpawnDebugGrid_SetIsOffshoot(adjacentGridItems[i], .1f);

		auto world = GetWorld();
		auto owner = this;
		FindPrimaryRouteBetweenRecursive(newRoute, end, maxRoute, routeId, bDebug);
	}
}

TArray<FVector> UBaseLevelIndex::GetAdjacentGridItems(FVector item)
{

	auto left = FVector(item.X - 100, item.Y, 0);
	auto right = FVector(item.X + 100, item.Y, 0);
	auto up = FVector(item.X, item.Y - 100, 0);
	auto down = FVector(item.X, item.Y + 100, 0);

	TArray<FVector> result;

	if (left.X <= MaxX * 100 && left.Y <= MaxY * 100 && left.X >= 0 && left.Y >= 0)
		result.Emplace(left);

	if (right.X <= MaxX * 100 && right.Y <= MaxY * 100 && right.X >= 0 && right.Y >= 0)
		result.Emplace(right);

	if (up.X <= MaxX * 100 && up.Y <= MaxY * 100 && up.X >= 0 && up.Y >= 0)
		result.Emplace(up);

	if (down.X <= MaxX * 100 && down.Y <= MaxY * 100 && down.X >= 0 && down.Y >= 0)
		result.Emplace(down);

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

bool UBaseLevelIndex::IsAValidRouteItem(FVector item)
{
	if (IsWithinList(EndZone, item) || IsWithinList(SpawnZone, item))
		return false;

	return true;
}

bool UBaseLevelIndex::IsWithinList(TArray<FVector> locations, FVector loc)
{
	for (int i = 0; i < locations.Num(); i++)
		if (locations[i] == loc)
			return true;

	return false;
}

bool UBaseLevelIndex::IsAnyWithinList(TArray<FVector> locations, TArray<FVector> testLocs)
{
	for (int i = 0; i < testLocs.Num(); i++)
		if (IsWithinList(locations, testLocs[i]))
			return true;

	return false;
}

bool UBaseLevelIndex::CanReachDestination(FVector location, FVector end, int steps)
{
	auto stepBuffer = 3;

	auto xmovement = location.X - end.X;
	if (xmovement < 0)
		xmovement *= -1;

	auto ymovement = location.Y - end.Y;
	if (ymovement < 0)
		ymovement *= -1;

	if ((ymovement + xmovement) / 100 <= (steps - stepBuffer))
	{
		return true;
	}
	else
		return false;
}

bool UBaseLevelIndex::IsFurtherFromEnd(FVector last, FVector current, FVector end)
{
	auto distFromLast = FVector::Dist(last, end);
	auto distFromNew = FVector::Dist(current, end);

	auto difference = distFromNew - distFromLast;
	// UDebugMessages::LogDisplay(this, "diff " + FString::SanitizeFloat(difference, 2));
	return difference > 75;
}

void UBaseLevelIndex::SetRouteById(int routeId, TArray<FVector> route)
{
	if (routeId == 1)
		PrimaryRoute = route;
	else if (routeId == 2)
		OffshootRoute1 = route;
	else if (routeId == 3)
		OffshootRoute2 = route;
	else if (routeId == 4)
		OffshootRoute3 = route;
	else if (routeId == 5)
		OffshootRoute4 = route;
	else if (routeId == 6)
		OffshootRoute5 = route;
}
TArray<FVector> UBaseLevelIndex::GetRouteById(int routeId)
{
	if (routeId == 1)
		return PrimaryRoute;
	else if (routeId == 2)
		return OffshootRoute1;
	else if (routeId == 3)
		return OffshootRoute2;
	else if (routeId == 4)
		return OffshootRoute3;
	else if (routeId == 5)
		return OffshootRoute4;
	else if (routeId == 6)
		return OffshootRoute5;

	return PrimaryRoute;
}

TArray<FVector> UBaseLevelIndex::GetRandomLocationAlongPrimaryRoute()
{
	TArray<FVector> route;
	auto gridItem = PrimaryRoute[RandomStream.RandRange(0, PrimaryRoute.Num() - 1)];
	gridItem.Z = 0;
	route.Emplace(gridItem);
	return route;
}

TArray<FVector> UBaseLevelIndex::GetTotalRoutesList()
{
	TArray<FVector> total;

	for (int i = 0; i < SpawnZone.Num(); i++)
	{
		total.Emplace(SpawnZone[i]);
	}
	for (int i = 0; i < EndZone.Num(); i++)
	{
		total.Emplace(EndZone[i]);
	}
	for (int i = 0; i < PrimaryRoute.Num(); i++)
	{
		total.Emplace(PrimaryRoute[i]);
	}
	for (int i = 0; i < OffshootRoute1.Num(); i++)
	{
		total.Emplace(OffshootRoute1[i]);
	}
	for (int i = 0; i < OffshootRoute2.Num(); i++)
	{
		total.Emplace(OffshootRoute2[i]);
	}
	for (int i = 0; i < OffshootRoute3.Num(); i++)
	{
		total.Emplace(OffshootRoute3[i]);
	}
	for (int i = 0; i < OffshootRoute4.Num(); i++)
	{
		total.Emplace(OffshootRoute4[i]);
	}
	for (int i = 0; i < OffshootRoute5.Num(); i++)
	{
		total.Emplace(OffshootRoute5[i]);
	}
	return total;
}

FVector UBaseLevelIndex::GetRandomLocationWithinGrid()
{
	auto gridItems = GridData.GetGridItems();
	auto gridItem = gridItems[RandomStream.RandRange(0, gridItems.Num() - 1)];
	auto location = gridItem.GetLocation();
	location.Z = 0;
	return location;
}

void UBaseLevelIndex::GenerateCaveWalls()
{
	auto gridItems = GridData.GetGridItems();
	auto totalRoute = GetTotalRoutesList();

	for (FGridDataItem dataItem : GridData.GetGridItems())
	{
		if (dataItem.GetLocation().Z != 0)
			continue;

		if (!dataItem.GetIsPotentialWallSpace())
			continue;

		TArray<FVector> cardinals;
		cardinals.Emplace(dataItem.GetLocation() + FVector(0, -300, 0));
		cardinals.Emplace(dataItem.GetLocation() + FVector(0, -200, 0));
		cardinals.Emplace(dataItem.GetLocation() + FVector(0, -100, 0));
		cardinals.Emplace(dataItem.GetLocation() + FVector(0, 300, 0));
		cardinals.Emplace(dataItem.GetLocation() + FVector(0, 200, 0));
		cardinals.Emplace(dataItem.GetLocation() + FVector(0, 100, 0));
		cardinals.Emplace(dataItem.GetLocation() + FVector(-300, 0, 0));
		cardinals.Emplace(dataItem.GetLocation() + FVector(-200, 0, 0));
		cardinals.Emplace(dataItem.GetLocation() + FVector(-100, 0, 0));
		cardinals.Emplace(dataItem.GetLocation() + FVector(300, 0, 0));
		cardinals.Emplace(dataItem.GetLocation() + FVector(200, 0, 0));
		cardinals.Emplace(dataItem.GetLocation() + FVector(100, 0, 0));

		bool allCardinalSpotsAreGood = true;
		for (FVector cardinal : cardinals)
		{
			auto foundGridItem = GridData.GetDataItem(cardinal);
			if (foundGridItem && (foundGridItem->GetIsBuffer() || foundGridItem->GetIsPrefab() ||
								  foundGridItem->GetIsRoad() || foundGridItem->GetIsStartZone() ||
								  foundGridItem->GetIsEndZone()))
			{
				UDebugMessages::LogDisplay(this, "grid item is invalid");
				allCardinalSpotsAreGood = false;
				break;
			}
		}

		if (allCardinalSpotsAreGood)
		{
			UDebugMessages::LogDisplay(this, "location is good");
			WallLocations.Emplace(dataItem.GetLocation());
		}
	}

	AllWallLocations = WallLocations;
	TArray<int> indexesToRemove;
	TArray<FVector> voidWallLocations;
	for (int i = 0; i < WallLocations.Num(); i++)
		if (CaveWallIsFullyEnclosed(WallLocations[i], WallLocations))
		{
			voidWallLocations.Emplace(WallLocations[i]);
			indexesToRemove.Emplace(i);
		}

	SetLocationsAsVoid(voidWallLocations);

	for (int i = WallLocations.Num(); i >= 0; i--)
		for (int j = 0; j < indexesToRemove.Num(); j++)
		{
			if (i == indexesToRemove[j])
			{
				WallLocations.RemoveAt(i);
				break;
			}
		}

	ObstacleAllowedLocations = RemoveListFromList(ObstacleAllowedLocations, AllWallLocations);
	UDebugMessages::LogDisplay(this, "Start Cave Walls");
	auto world = GetWorld();
	for (int i = 0; i < WallLocations.Num(); i++)
	{
		auto gridItem = GridData.GetDataItem(WallLocations[i]);
		if (gridItem)
			gridItem->SetIsWall();

		auto thisLoc = WallLocations[i];
		FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, thisLoc]
																				  {
			//auto actor = world->SpawnActor<ARockSection>(thisLoc + FVector(50, 50, 0), FRotator::ZeroRotator);
			auto actor = world->SpawnActor<ARockSection>(thisLoc, FRotator::ZeroRotator); }, TStatId(), nullptr, ENamedThreads::GameThread);
	}

	UDebugMessages::LogDisplay(this, "Cave Walls");
}
void UBaseLevelIndex::GenerateCaveWalls_Old()
{
	auto gridItems = GridData.GetGridItems();
	auto totalRoute = GetTotalRoutesList();

	for (int i = 0; i < Grid.Num(); i++)
	{

		if (Grid[i].Z != 0)
			continue;

		auto location = gridItems[i].GetLocation();

		if (IsWithinList(PrimaryRoute, location) || IsWithinList(OffshootRoute2, location) ||
			IsWithinList(OffshootRoute3, location) || IsWithinList(OffshootRoute4, location) ||
			IsWithinList(OffshootRoute5, location))
		{
			continue;
		}

		TArray<FVector> cardinals;
		cardinals.Emplace(location + FVector(0, -300, 0));
		cardinals.Emplace(location + FVector(0, -200, 0));
		cardinals.Emplace(location + FVector(0, -100, 0));
		cardinals.Emplace(location + FVector(0, 300, 0));
		cardinals.Emplace(location + FVector(0, 200, 0));
		cardinals.Emplace(location + FVector(0, 100, 0));
		cardinals.Emplace(location + FVector(-300, 0, 0));
		cardinals.Emplace(location + FVector(-200, 0, 0));
		cardinals.Emplace(location + FVector(-100, 0, 0));
		cardinals.Emplace(location + FVector(300, 0, 0));
		cardinals.Emplace(location + FVector(200, 0, 0));
		cardinals.Emplace(location + FVector(100, 0, 0));

		if (IsAnyWithinList(totalRoute, cardinals))
			continue;

		WallLocations.Emplace(location);
	}

	AllWallLocations = WallLocations;
	TArray<int> indexesToRemove;
	TArray<FVector> voidWallLocations;
	for (int i = 0; i < WallLocations.Num(); i++)
		if (CaveWallIsFullyEnclosed(WallLocations[i], WallLocations))
		{
			voidWallLocations.Emplace(WallLocations[i]);
			indexesToRemove.Emplace(i);
		}

	SetLocationsAsVoid(voidWallLocations);

	for (int i = WallLocations.Num(); i >= 0; i--)
		for (int j = 0; j < indexesToRemove.Num(); j++)
		{
			if (i == indexesToRemove[j])
			{
				WallLocations.RemoveAt(i);
				break;
			}
		}

	auto world = GetWorld();
	for (int i = 0; i < WallLocations.Num(); i++)
	{
		// auto gridItem = GridData.GetDataItem(WallLocations[i]);
		// if (gridItem)
		// 	gridItem->SetIsWall();

		auto thisLoc = WallLocations[i];
		FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, thisLoc]
																				  {
			//auto actor = world->SpawnActor<ARockSection>(thisLoc + FVector(50, 50, 0), FRotator::ZeroRotator);
			auto actor = world->SpawnActor<ARockSection>(thisLoc, FRotator::ZeroRotator); }, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}
bool UBaseLevelIndex::CaveWallIsFullyEnclosed(FVector wall, TArray<FVector> allWalls)
{
	auto left = wall + FVector(0, -100, 0);
	auto right = wall + FVector(0, 100, 0);
	auto up = wall + FVector(-100, 0, 0);
	auto down = wall + FVector(100, 0, 0);

	if (IsWithinList(allWalls, left) &&
		IsWithinList(allWalls, right) &&
		IsWithinList(allWalls, up) &&
		IsWithinList(allWalls, down))
	{
		return true;
	}

	return false;
}

void UBaseLevelIndex::FillAllObstacleAllowedLocations()
{
	auto gridItems = GridData.GetGridItems();
	for (int i = 0; i < gridItems.Num(); i++)
	{
		if (!IsWithinList(SpawnZone, gridItems[i].GetLocation()) &&
			!IsWithinList(EndZone, gridItems[i].GetLocation()) &&
			!IsWithinList(AllWallLocations, gridItems[i].GetLocation()) &&
			gridItems[i].GetLocation().Z == 0)
		{
			ObstacleAllowedLocations.Emplace(gridItems[i].GetLocation());
		}
	}
}

FVector UBaseLevelIndex::GetRandomStartZoneLocation()
{
	return SpawnZone[RandomStream.RandRange(0, SpawnZone.Num() - 1)];
}
FVector UBaseLevelIndex::GetRandomEndZoneLocation()
{
	return EndZone[RandomStream.RandRange(0, EndZone.Num() - 1)];
}
FVector UBaseLevelIndex::GetRandomLocationInList(TArray<FVector> locs)
{
	return locs[RandomStream.RandRange(0, locs.Num() - 1)];
}

FVector UBaseLevelIndex::GetClosestAllowedLocation(FVector loc)
{
	float distance = -1;
	FVector resultingLoc = FVector::ZeroVector;

	for (int i = 0; i < ObstacleAllowedLocations.Num(); i++)
	{
		auto thisDist = FVector::Dist(loc, ObstacleAllowedLocations[i]);
		if (distance < 0 || thisDist < distance)
		{
			distance = thisDist;
			resultingLoc = ObstacleAllowedLocations[i];
		}
	}

	return resultingLoc;
}

void UBaseLevelIndex::SpawnDebugGrid_SetIsStart(FVector location, float delay)
{
	auto world = GetWorld();
	FPlatformProcess::Sleep(delay);
	location += FVector(-50, -50, 0);
	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, location]
																			  {
		auto actor = world->SpawnActor<AWorldGridItemActor>(location, FRotator::ZeroRotator);
		actor->SetIsStart(); }, TStatId(), nullptr, ENamedThreads::GameThread);
}
void UBaseLevelIndex::SpawnDebugGrid_SetIsEnd(FVector location, float delay)
{
	auto world = GetWorld();
	FPlatformProcess::Sleep(delay);
	location += FVector(-50, -50, 0);
	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, location]
																			  {
		auto actor = world->SpawnActor<AWorldGridItemActor>(location, FRotator::ZeroRotator);
		actor->SetIsEnd(); }, TStatId(), nullptr, ENamedThreads::GameThread);
}
void UBaseLevelIndex::SpawnDebugGrid_SetIsOffshoot(FVector location, float delay)
{
	auto world = GetWorld();
	FPlatformProcess::Sleep(delay);
	location += FVector(-50, -50, 0);
	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, location]
																			  {
		auto actor = world->SpawnActor<AWorldGridItemActor>(location, FRotator::ZeroRotator);
		actor->SetIsOffshoot(); }, TStatId(), nullptr, ENamedThreads::GameThread);
}
void UBaseLevelIndex::SpawnDebugGrid_SetIsSpawn(FVector location, float delay)
{
	auto world = GetWorld();
	FPlatformProcess::Sleep(delay);
	location += FVector(-50, -50, 0);
	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, location]
																			  {
		auto actor = world->SpawnActor<AWorldGridItemActor>(location, FRotator::ZeroRotator);
		actor->SetIsSpawn(); }, TStatId(), nullptr, ENamedThreads::GameThread);
}
void UBaseLevelIndex::SpawnDebugGrid_SetIsObstacle(FVector location, float delay)
{
	auto world = GetWorld();
	FPlatformProcess::Sleep(delay);
	location += FVector(-50, -50, 0);
	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, location]
																			  {
		auto actor = world->SpawnActor<AWorldGridItemActor>(location, FRotator::ZeroRotator);
		actor->SetIsObstacle(); }, TStatId(), nullptr, ENamedThreads::GameThread);
}

TArray<FVector> UBaseLevelIndex::RemoveListFromList(TArray<FVector> bigList, TArray<FVector> removesThisListFromItemsFromBigList)
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

TArray<FVector> UBaseLevelIndex::CombineList(TArray<FVector> locs1, TArray<FVector> locs2)
{
	for (int i = 0; i < locs1.Num(); i++)
	{
		locs2.Emplace(locs1[i]);
	}
	return locs2;
}

void UBaseLevelIndex::DebugGenerations()
{
	auto world = GetWorld();
	for (FGridDataItem dataItem : GridData.GetGridItems())
	{
		auto location = dataItem.GetLocation();

		if (!dataItem.GetIsBuffer() && !dataItem.GetIsPrefab() && !dataItem.GetIsVoid() &&
			!dataItem.GetIsRoad() && !dataItem.GetIsWall() && !dataItem.GetIsDebris() && dataItem.GetLocation().Z == 0)
		{
			FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
				[world, location]
				{
					auto actor = world->SpawnActor<AWorldGridItemActor>(location, FRotator::ZeroRotator);
					actor->SetIsStart();
				},
				TStatId(), nullptr, ENamedThreads::GameThread);
			continue;
		}

		if (dataItem.GetIsRoad())
		{
			FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
				[world, location]
				{
					auto actor = world->SpawnActor<AWorldGridItemActor>(location, FRotator::ZeroRotator);
					actor->SetIsOffshoot();
				},
				TStatId(), nullptr, ENamedThreads::GameThread);
		}

		if (dataItem.GetIsBuffer())
		{
			FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
				[world, location]
				{
					auto actor = world->SpawnActor<AWorldGridItemActor>(location, FRotator::ZeroRotator);
					actor->SetIsEnd();
				},
				TStatId(), nullptr, ENamedThreads::GameThread);
		}

		if (dataItem.GetIsPrefab())
		{
			FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
				[world, location]
				{
					auto actor = world->SpawnActor<AWorldGridItemActor>(location, FRotator::ZeroRotator);
					actor->SetIsObstacle();
				},
				TStatId(), nullptr, ENamedThreads::GameThread);
		}

		if (dataItem.GetIsWall())
		{
			FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
				[world, location]
				{
					auto actor = world->SpawnActor<AWorldGridItemActor>(location, FRotator::ZeroRotator);
					actor->SetIsWall();
				},
				TStatId(), nullptr, ENamedThreads::GameThread);
		}

		if (dataItem.GetIsDebris())
		{
			FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
				[world, location]
				{
					auto actor = world->SpawnActor<AWorldGridItemActor>(location, FRotator::ZeroRotator);
					actor->SetIsDebris();
				},
				TStatId(), nullptr, ENamedThreads::GameThread);
		}
	}
}

void UBaseLevelIndex::SetLocationsAsBuffer(TArray<FVector> locs)
{
	for (int i = 0; i < locs.Num(); i++)
	{
		auto item = GridData.GetDataItem(locs[i]);
		if (item)
			item->SetIsBuffer();
		else
			UDebugMessages::LogError(this, "SetIsBuffer but could not find in grid data");
	}
}
void UBaseLevelIndex::SetLocationsAsPrefab(TArray<FVector> locs)
{
	for (int i = 0; i < locs.Num(); i++)
	{
		auto item = GridData.GetDataItem(locs[i]);
		if (item)
			item->SetIsPrefab();
		else
			UDebugMessages::LogError(this, "SetIsBuffer but could not find in grid data");
	}
}
void UBaseLevelIndex::SetLocationsAsRoad(TArray<FVector> locs)
{
	for (int i = 0; i < locs.Num(); i++)
	{
		auto item = GridData.GetDataItem(locs[i]);
		if (item)
			item->SetIsRoad();
		else
			UDebugMessages::LogError(this, "SetIsBuffer but could not find in grid data");
	}
}

void UBaseLevelIndex::SetLocationsAsVoid(TArray<FVector> locs)
{
	for (int i = 0; i < locs.Num(); i++)
	{
		auto item = GridData.GetDataItem(locs[i]);
		if (item)
			item->SetIsVoid();
		else
			UDebugMessages::LogError(this, "SetIsBuffer but could not find in grid data");
	}
}

void UBaseLevelIndex::SetLocationsAsDebris(TArray<FVector> locs)
{
	for (int i = 0; i < locs.Num(); i++)
	{
		auto item = GridData.GetDataItem(locs[i]);
		if (item)
			item->SetIsDebris();
		else
			UDebugMessages::LogError(this, "SetIsBuffer but could not find in grid data");
	}
}

void UBaseLevelIndex::SetLocationsAsSpawnZone(TArray<FVector> locs)
{
	for (int i = 0; i < locs.Num(); i++)
	{
		auto item = GridData.GetDataItem(locs[i]);
		if (item)
			item->SetIsStartZone();
		else
			UDebugMessages::LogError(this, "SetIsBuffer but could not find in grid data");
	}
}
void UBaseLevelIndex::SetLocationsAsEndZone(TArray<FVector> locs)
{
	for (int i = 0; i < locs.Num(); i++)
	{
		auto item = GridData.GetDataItem(locs[i]);
		if (item)
			item->SetIsEndZone();
		else
			UDebugMessages::LogError(this, "SetIsBuffer but could not find in grid data");
	}
}