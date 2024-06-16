// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerationRunnable.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Instance/SvGameInstance.h"
#include "../Instance/Managers/LevelGenerationManager.h"
#include "../GameModes/GameplayMode.h"
#include "../GameModes/Managers/LevelSpawnerManager.h"
#include "../Utilities/RunnableUtilities.h"
#include "../World/WorldGridItemActor.h"
#include "VgCore/Domain/Debug/DebugMessages.h"


#pragma optimize("", off)
void ULevelGenerationRunnable::ActivateThread() {
	int minX = 20;
	MaxX = 40;
	int minY = 20;
	MaxY = 40;

	int XBorder = RandomStream.RandRange(minX, MaxX);
	int YBorder = RandomStream.RandRange(minY, MaxY);

	//create full grid list of 0 to max of each border
	CreateGrid(MaxX, MaxY);
	//set spawn zone and completion zone
	SetSpawnAndEndZone();
	//generate boundary walls
	GenerateBoundaryWalls();

	TArray<FVector> spawnArr;
	spawnArr.Emplace(SpawnZone[RandomStream.RandRange(0, SpawnZone.Num() - 1)]);

	TArray<FVector> endArr;
	endArr.Emplace(EndZone[RandomStream.RandRange(0, EndZone.Num() - 1)]);

	//auto mainRoute = FindRouteBetweenRecursive(spawnArr, EndZone[RandomStream.RandRange(0, EndZone.Num() - 1)], TwoDGrid, MaxX + MaxY);

	auto world = GetWorld();
	auto spawnLocs = SpawnZone;
	FGraphEventRef spawnTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, spawnLocs] {

		for (int i = 0; i < spawnLocs.Num(); i++) {
			auto actor = world->SpawnActor<AWorldGridItemActor>(spawnLocs[i], FRotator::ZeroRotator);
			actor->SetIsSpawn();
		}
		}, TStatId(), nullptr, ENamedThreads::GameThread);

	auto endLocs = EndZone;
	FGraphEventRef endTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, endLocs] {
		for (int i = 0; i < endLocs.Num(); i++) {
			auto actor = world->SpawnActor<AWorldGridItemActor>(endLocs[i], FRotator::ZeroRotator);
			actor->SetIsEnd();
		}
		}, TStatId(), nullptr, ENamedThreads::GameThread);


	FindPrimaryRouteBetweenRecursive(spawnArr, EndZone[RandomStream.RandRange(0, EndZone.Num() - 1)], 80);

	for (int i = 0; i < PrimaryRoute.Num(); i++) {
		auto thisRoute = PrimaryRoute[i];
		FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, thisRoute] {
			auto actor = world->SpawnActor<AWorldGridItemActor>(thisRoute, FRotator::ZeroRotator);
			}, TStatId(), nullptr, ENamedThreads::GameThread);
	}

	FPlatformProcess::Sleep(1);
}

ULevelGenerationRunnable* ULevelGenerationRunnable::InsertVariables() {
	return this;
}

void ULevelGenerationRunnable::AddDebugActor(AActor* actor) {
	DebugActors.Emplace(actor);
}

void ULevelGenerationRunnable::CreateGrid(int maxX, int maxY, int elevation) {
	for (int j = 0; j <= elevation; j++) {
		for (int i = 0; i <= maxX; i++) {
			for (int x = 0; x <= maxY; x++) {
				Grid.Emplace(FVector(i * 100, x * 100, j * 200));

				if (j == 0)
					TwoDGrid.Emplace(FVector2D(i, x));
			}
		}
	}
}
void ULevelGenerationRunnable::SetSpawnAndEndZone() {
	int random = RandomStream.RandRange(1, 1001);

	auto spawnPosStartX = (MaxX / 2) - 3;
	auto spawnPosEndX = (MaxX / 2) + 3;
	auto spawnPosStartY = 0;
	auto spawnPosEndY = 1;

	auto endPosStartX = (MaxX / 2) - 3;
	auto endPosEndX = (MaxX / 2) + 3;
	auto endPosStartY = MaxY - 1;
	auto endPosEndY = MaxY;

	if (random < 500) {
		spawnPosStartY = MaxY - 1;
		spawnPosEndY = MaxY;

		endPosStartY = 0;
		endPosEndY = 1;
	}
	else if (random < 750) {
		spawnPosStartX = 0;
		spawnPosEndX = 1;

		spawnPosStartY = (MaxY / 2) - 3;
		spawnPosEndY = (MaxY / 2) + 3;

		endPosStartY = (MaxY / 2) - 3;
		endPosEndY = (MaxY / 2) + 3;
		endPosStartX = MaxX - 1;
		endPosEndX = MaxX;
	}
	else {
		spawnPosStartX = MaxX - 1;
		spawnPosEndX = MaxX;

		spawnPosStartY = (MaxY / 2) - 3;
		spawnPosEndY = (MaxY / 2) + 3;

		endPosStartY = (MaxY / 2) - 3;
		endPosEndY = (MaxY / 2) + 3;

		endPosEndX = 1;
		endPosStartX = 0;
	}

	for (int x = spawnPosStartX; x <= spawnPosEndX; x++) {
		for (int y = spawnPosStartY; y <= spawnPosEndY; y++) {
			SpawnZone.Emplace(FVector(x * 100, y * 100, 0));
		}
	}

	for (int x = endPosStartX; x <= endPosEndX; x++) {
		for (int y = endPosStartY; y <= endPosEndY; y++) {
			EndZone.Emplace(FVector(x * 100, y * 100, 0));
		}
	}
}
void ULevelGenerationRunnable::GenerateBoundaryWalls() {
	auto gameMode = GetWorld()->GetAuthGameMode<AGameplayMode>();
	auto spawnerManager = gameMode->GetLevelSpawnerManager();

	for (int i = 0; i < Grid.Num(); i++) {
		auto thisLocation = Grid[i];
		//bottom left corner
		if (Grid[i].X == 0 && Grid[i].Y == 0) {
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([spawnerManager, thisLocation] {
				spawnerManager->SpawnMajorWallAt(thisLocation, FRotator(0, 90, 0));
				}, TStatId(), nullptr, ENamedThreads::GameThread);
		}
		//bottom right corner
		else if (Grid[i].Y == MaxY * 100 && Grid[i].X == 0) {
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([spawnerManager, thisLocation] {
				spawnerManager->SpawnMajorWallAt(thisLocation, FRotator(0, 0, 0));
				spawnerManager->SpawnMajorWallAt(thisLocation + FVector(0, 100, 0), FRotator(0, 0, 0));
				spawnerManager->SpawnMajorWallAt(thisLocation + FVector(0, 100, 0), FRotator(0, 90, 0));
				}, TStatId(), nullptr, ENamedThreads::GameThread);
		}
		//top left corner
		else if (Grid[i].X == MaxX * 100 && Grid[i].Y == 0) {
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([spawnerManager, thisLocation] {
				spawnerManager->SpawnMajorWallAt(thisLocation, FRotator(0, 90, 0));
				}, TStatId(), nullptr, ENamedThreads::GameThread);
		}
		//top right corner
		else if (Grid[i].X == MaxX * 100 && Grid[i].Y == MaxY * 100) {
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([spawnerManager, thisLocation] {
				//spawnerManager->SpawnMajorWallAt(thisLocation + FVector(0, 100, 0), FRotator(0, 90, 0));
				spawnerManager->SpawnMajorWallAt(thisLocation + FVector(100, 0, 0), FRotator(0, 0, 0));
				spawnerManager->SpawnMajorWallAt(thisLocation + FVector(100, 100, 0), FRotator(0, 0, 0));

				spawnerManager->SpawnMajorWallAt(thisLocation + FVector(0, 100, 0), FRotator(0, 90, 0));

				}, TStatId(), nullptr, ENamedThreads::GameThread);
		}
		else if (Grid[i].X == 0) {
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([spawnerManager, thisLocation] {
				spawnerManager->SpawnMajorWallAt(thisLocation);
				}, TStatId(), nullptr, ENamedThreads::GameThread);
		}
		else if (Grid[i].X == MaxX * 100) {
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([spawnerManager, thisLocation] {
				spawnerManager->SpawnMajorWallAt(thisLocation + FVector(100, 0, 0));
				}, TStatId(), nullptr, ENamedThreads::GameThread);
		}
		else if (Grid[i].Y == 0) {
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([spawnerManager, thisLocation] {
				spawnerManager->SpawnMajorWallAt(thisLocation, FRotator(0, 90, 0));
				}, TStatId(), nullptr, ENamedThreads::GameThread);
		}
		else if (Grid[i].Y == MaxY * 100) {
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([spawnerManager, thisLocation] {
				spawnerManager->SpawnMajorWallAt(thisLocation + FVector(0, 100, 0), FRotator(0, 90, 0));
				}, TStatId(), nullptr, ENamedThreads::GameThread);
		}
	}
}

void ULevelGenerationRunnable::FindPrimaryRouteBetweenRecursive(TArray<FVector> currentRoute, FVector end, int maxRoute) {
	if (currentRoute.Num() > maxRoute) 
		return;

	auto lastRoute = currentRoute[currentRoute.Num() - 1];
	auto adjacentGridItems = GetAdjacentGridItems(lastRoute);

	for (int i = 0; i < adjacentGridItems.Num(); i++) {
		if (!PrimaryRoute.IsEmpty())
			return;

		//if (!IsAValidRouteItem(adjacentGridItems[i]))
		//	continue;

		if (IsWithinList(currentRoute, adjacentGridItems[i]))
			continue;

		if (!CanReachDestination(adjacentGridItems[i], end, maxRoute - currentRoute.Num()))
			continue;

		if (IsFurtherFromEnd(lastRoute, adjacentGridItems[i], end))
			continue;

		if (adjacentGridItems[i] == end) {
			for (int x = 0; x < currentRoute.Num(); x++)
				PrimaryRoute.Emplace(currentRoute[x]);

			PrimaryRoute.Emplace(adjacentGridItems[i]);
			return;
		}

		auto newRoute = currentRoute;
		newRoute.Emplace(adjacentGridItems[i]);

		auto world = GetWorld();
		auto owner = this;
		FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, newRoute, owner] {

			for (int i = 0; i < owner->DebugActors.Num(); i++) {
				if (owner->DebugActors[i])
					owner->DebugActors[i]->Destroy();
			}
			owner->DebugActors.Empty();

			for (int i = 0; i < newRoute.Num(); i++) {
				auto actor = world->SpawnActor<AWorldGridItemActor>(newRoute[i], FRotator::ZeroRotator);
				if (actor) {
					owner->AddDebugActor(actor);
				}
			}

			}, TStatId(), nullptr, ENamedThreads::GameThread);
		FPlatformProcess::Sleep(.5f);

		FindPrimaryRouteBetweenRecursive(newRoute, end, maxRoute);
	}
}

TArray<FVector> ULevelGenerationRunnable::GetAdjacentGridItems(FVector item) {

	auto left = FVector(item.X - 100, item.Y, 0);
	auto right = FVector(item.X + 100, item.Y, 0);
	auto up = FVector(item.X, item.Y - 100, 0);
	auto down = FVector(item.X, item.Y + 100, 0);

	TArray<FVector> result;

	if (left.X <= MaxX * 100 && left.Y <= MaxY * 100)		result.Emplace(left);
	if (right.X <= MaxX * 100 && right.Y <= MaxY * 100)		result.Emplace(right);
	if (up.X <= MaxX * 100 && up.Y <= MaxY * 100)			result.Emplace(up);
	if (down.X <= MaxX * 100 && down.Y <= MaxY * 100)		result.Emplace(down);

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


bool ULevelGenerationRunnable::IsAValidRouteItem(FVector item) {
	if (IsWithinList(EndZone, item) || IsWithinList(SpawnZone, item))
		return false;

	return true;
}

bool ULevelGenerationRunnable::IsWithinList(TArray<FVector> locations, FVector loc) {
	for (int i = 0; i < locations.Num(); i++)
		if (locations[i] == loc)
			return true;

	return false;
}

bool ULevelGenerationRunnable::CanReachDestination(FVector location, FVector end, int steps) {

	auto xmovement = location.X - end.X;
	if (xmovement < 0) xmovement *= -1;

	auto ymovement = location.Y - end.Y;
	if (ymovement < 0) ymovement *= -1;

	if ((ymovement + xmovement) / 100 <= steps) {
		return true;
	}
	else return false;
}

bool ULevelGenerationRunnable::IsFurtherFromEnd(FVector last, FVector current, FVector end) {
	auto distFromLast = FVector::Dist(last, end);
	auto distFromNew = FVector::Dist(current, end);

	auto difference = distFromNew - distFromLast;
	UDebugMessages::LogDisplay(this, "diff " + FString::SanitizeFloat(difference, 2));
	return difference > 75;
}

#pragma optimize("", on)