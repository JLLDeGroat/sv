// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericLevel.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../../Instance/SvGameInstance.h"
#include "../../../Instance/Managers/LevelGenerationManager.h"
#include "../../../GameModes/Managers/LevelSpawnerManager.h"
#include "../../../Utilities/RunnableUtilities.h"
#include "../../../World/WorldGridItemActor.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Generations/FenceGenerations.h"
#include "../../Generations/BuildingGeneration.h"
#include "../../Generations/RoadGenerations.h"
#include "../../Generations/EnemyGeneration.h"
#include "../../Generations/PlayerGeneration.h"
#include "../../Generations/StaticSpawnerGeneration.h"
#include "../../../Environment/Natural/RockSection.h"


void UGenericLevel::GenerateLevel() {
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

	bool bDebug = true;
	if (bDebug)
		return UDebugMessages::LogError(this, "Manual return out of level gen, this is for debug only");

	TArray<FVector> spawnArr;
	spawnArr.Emplace(GetRandomStartZoneLocation());

	TArray<FVector> endArr;
	endArr.Emplace(GetRandomEndZoneLocation());

	auto world = GetWorld();

	FindPrimaryRouteBetweenRecursive(spawnArr, EndZone[RandomStream.RandRange(0, EndZone.Num() - 1)], 80, 1);
	FindPrimaryRouteBetweenRecursive(GetRandomLocationAlongPrimaryRoute(), GetRandomLocationWithinGrid(), 90, 2);
	FindPrimaryRouteBetweenRecursive(GetRandomLocationAlongPrimaryRoute(), GetRandomLocationWithinGrid(), 90, 3);
	FindPrimaryRouteBetweenRecursive(GetRandomLocationAlongPrimaryRoute(), GetRandomLocationWithinGrid(), 90, 4);
	FindPrimaryRouteBetweenRecursive(GetRandomLocationAlongPrimaryRoute(), GetRandomLocationWithinGrid(), 90, 5);
	FindPrimaryRouteBetweenRecursive(GetRandomLocationAlongPrimaryRoute(), GetRandomLocationWithinGrid(), 90, 6);

	GenerateCaveWalls();
	FillAllObstacleAllowedLocations();

	/*for (int i = 0; i < ObstacleAllowedLocations.Num(); i++)
		SpawnDebugGrid_SetIsObstacle(ObstacleAllowedLocations[i]);*/

	auto fenceGen = NewObject<UFenceGenerations>(this)
		->SetupGeneration(GetWorld(), RandomStream, ObstacleAllowedLocations)
		->Generate();
	ObstacleAllowedLocations = RemoveListFromList(ObstacleAllowedLocations, fenceGen->GetRequiredLocations());
	//ObstacleAllowedLocations = fenceGen->GetUnusedSpotsLeft();

	//for (int i = 0; i < ObstacleAllowedLocations.Num(); i++)
	//	SpawnDebugGrid_SetIsObstacle(ObstacleAllowedLocations[i]);

	auto buildinGen = NewObject<UBuildingGeneration>(this)
		->SetupGeneration(GetWorld(), RandomStream, ObstacleAllowedLocations)
		->Generate();
	ObstacleAllowedLocations = RemoveListFromList(ObstacleAllowedLocations, buildinGen->GetRequiredLocations());
	//ObstacleAllowedLocations = buildinGen->GetUnusedSpotsLeft();

	/*for (int i = 0; i < ObstacleAllowedLocations.Num(); i++) {
		SpawnDebugGrid_SetIsObstacle(ObstacleAllowedLocations[i] + FVector(0, 0, -50));
	}*/

	auto closestAllowedStartLocation = GetClosestAllowedLocation(GetRandomStartZoneLocation());
	auto closestAllowedEndLocation = GetClosestAllowedLocation(GetRandomStartZoneLocation());
	auto roadGen = NewObject<URoadGenerations>(this)
		->SetupGeneration(GetWorld(), RandomStream, ObstacleAllowedLocations)
		->SetRoadGenStartAndEndLocation(closestAllowedStartLocation, closestAllowedEndLocation)
		->SetStartAndEndZones(SpawnZone, EndZone)
		->Generate();

	UDebugMessages::LogDisplay(this, "Road Generation complete");

	auto enemyGen = NewObject<UEnemyGeneration>(this)
		->SetTotalWalkingZombies(10)
		->SetupGeneration(GetWorld(), RandomStream, ObstacleAllowedLocations)
		->SetStartAndEndZones(SpawnZone, EndZone)
		->Generate();

	auto playerGen = NewObject<UPlayerGeneration>(this)
		->SetupGeneration(GetWorld(), RandomStream, ObstacleAllowedLocations)
		->SetStartAndEndZones(SpawnZone, EndZone)
		->Generate();

	auto staticSpawnerGen = NewObject<UStaticSpawnerGeneration>(this)
		->SetAmountToSpawn(10)
		->SetupGeneration(GetWorld(), RandomStream, ObstacleAllowedLocations)
		->SetStartAndEndZones(SpawnZone, EndZone)
		->Generate();
}