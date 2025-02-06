// Fill out your copyright notice in the Description page of Project Settings.


#include "TwoBuildingsLevel.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../BuildingGeneration.h"
#include "../../../World/WorldGridItemActor.h"
#include "../RoadGenerations.h"
#include "../../../Utilities/SvUtilities.h"
#include "../../Generations/EnemyGeneration.h"
#include "../../Generations/PlayerGeneration.h"
#include "../../Generations/StaticSpawnerGeneration.h"


void UTwoBuildingsLevel::GenerateLevel() {
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

	bool bDebug = false;
	if (bDebug)
		return UDebugMessages::LogError(this, "Manual return out of level gen, this is for debug only");

	TArray<FVector> spawnArr;
	spawnArr.Emplace(GetRandomStartZoneLocation());

	TArray<FVector> endArr;
	endArr.Emplace(GetRandomEndZoneLocation());

	auto world = GetWorld();

	int maxBuildingX = MaxX - 3;
	int maxBuildingY = MaxY - 3;

	for (int i = 3; i < maxBuildingX; i++) {
		for (int j = 3; j < maxBuildingY; j++) {
			ObstacleAllowedLocations.Emplace(FVector(i * 100, j * 100, 0));
		}
	}

	TArray<FVector> totalBuildingUsage;

	UDebugMessages::LogDisplay(this, "did basic generation");
	auto buildinGen = (UBuildingGeneration*)NewObject<UBuildingGeneration>(this)
		->SetXandYWidth(12, 12)
		->SetAmountToGenerate(2)
		->SetupGeneration(GetWorld(), RandomStream, USvUtilities::RandomizeList(ObstacleAllowedLocations))
		->Generate();

	totalBuildingUsage = buildinGen->GetRequiredLocations();
	FindRouteIgnoreLocations = buildinGen->GetRequiredLocations();
	ObstacleAllowedLocations = RemoveListFromList(ObstacleAllowedLocations, buildinGen->GetRequiredLocations());

	auto usedLocs = buildinGen->GetRequiredLocations();
	auto usedBuffers = buildinGen->GetRequiredBufferLocations();
	for (int i = 0; i < usedLocs.Num(); i++) {
		//SpawnDebugGrid_SetIsEnd(usedLocs[i]);
	}
	for (int i = 0; i < usedBuffers.Num(); i++) {
		//SpawnDebugGrid_SetIsSpawn(usedBuffers[i]);
	}

	UDebugMessages::LogDisplay(this, "completed building gen");
	auto doorLocs = buildinGen->GetDoorLocations();
	if (doorLocs.Num() == 1) {

		auto allowedSmallerBuildingLocations = ObstacleAllowedLocations;
		allowedSmallerBuildingLocations = RemoveListFromList(allowedSmallerBuildingLocations, buildinGen->GetRequiredBufferLocations());

		UDebugMessages::LogWarning(this, "failed to generate 2 buildings, manually creating another");
		auto secondaryBuildingGen = (UBuildingGeneration*)NewObject<UBuildingGeneration>(this)
			->SetXandYWidth(10, 10)
			->SetAmountToGenerate(1)
			->SetupGeneration(GetWorld(), RandomStream, USvUtilities::RandomizeList(allowedSmallerBuildingLocations))
			->Generate();

		doorLocs.Emplace(secondaryBuildingGen->GetDoorLocations()[0]);
		auto secondaryUsedLocs = secondaryBuildingGen->GetRequiredLocations();
		auto secondaryUsedBuffers = secondaryBuildingGen->GetRequiredBufferLocations();
		for (int i = 0; i < secondaryUsedLocs.Num(); i++) {
			//SpawnDebugGrid_SetIsEnd(secondaryUsedLocs[i]);
		}
		for (int i = 0; i < secondaryUsedBuffers.Num(); i++) {
			//SpawnDebugGrid_SetIsSpawn(secondaryUsedBuffers[i]);
		}

		FindRouteIgnoreLocations = CombineList(FindRouteIgnoreLocations, secondaryUsedLocs);
		ObstacleAllowedLocations = RemoveListFromList(ObstacleAllowedLocations, secondaryUsedLocs);
		totalBuildingUsage = CombineList(totalBuildingUsage, secondaryUsedLocs);
		/*for (int i = 0; i < FindRouteIgnoreLocations.Num(); i++) {
			SpawnDebugGrid_SetIsSpawn(FindRouteIgnoreLocations[i]);
		}*/
	}

	//for (int i = 0; i < FindRouteIgnoreLocations.Num(); i++) {
	//	SpawnDebugGrid_SetIsSpawn(FindRouteIgnoreLocations[i]);
	//}

	URoadGenerations* buildingToBuildingRoadGen = (URoadGenerations*)NewObject<URoadGenerations>(this);

	for (int i = 0; i < doorLocs.Num(); i++) {
		SpawnDebugGrid_SetIsObstacle(doorLocs[i]);
	}

	if (doorLocs.Num() > 1) {
		for (int i = 0; i < doorLocs.Num(); i++) {
			auto closestAllowedStartLoc = GetClosestAllowedLocation(doorLocs[i]);
			auto assumesLocationsHaveAmount = doorLocs.Num() > i + 1;

			//SpawnDebugGrid_SetIsStart(closestAllowedStartLoc + FVector(0, 0, 50));

			if (i == 0) {
				//Get route from start to this Door
				TArray<FVector> primaryRouteStage;
				auto randomStart = GetClosestAllowedLocation(GetRandomStartZoneLocation());
				//SpawnDebugGrid_SetIsStart(randomStart + FVector(0, 0, 50));
				primaryRouteStage.Emplace(randomStart);
				UDebugMessages::LogDisplay(this, "finding route from " + randomStart.ToString() + " to: " + closestAllowedStartLoc.ToString());
				FindPrimaryRouteBetweenRecursive(primaryRouteStage, closestAllowedStartLoc, 70 + (i * 20), 1, false);
				UDebugMessages::LogDisplay(this, "did 0 iteration loop");
			}

			if (assumesLocationsHaveAmount) {
				auto closestAllowedEndLoc = GetClosestAllowedLocation(doorLocs[i + 1]);

				auto lastOfPrimaryRouteStage = GetClosestAllowedLocation(PrimaryRoute[PrimaryRoute.Num() - 1]);
				TArray<FVector> routeStage2;
				routeStage2.Emplace(lastOfPrimaryRouteStage);
				UDebugMessages::LogDisplay(this, "finding route from " + lastOfPrimaryRouteStage.ToString() + " to: " + closestAllowedEndLoc.ToString());
				FindPrimaryRouteBetweenRecursive(routeStage2, closestAllowedEndLoc, 140 + (i * 20), 2, false);
				UDebugMessages::LogDisplay(this, "did stage 2 route loop");
				buildingToBuildingRoadGen
					->SetupGeneration(GetWorld(), RandomStream, ObstacleAllowedLocations)
					->SetRoadGenStartAndEndLocation(closestAllowedStartLoc, closestAllowedEndLoc)
					->SetStartAndEndZones(SpawnZone, EndZone)
					->Generate();
				UDebugMessages::LogDisplay(this, "generated roads");
			}
		}

		if (GetRouteById(2).IsEmpty()) {
			UDebugMessages::LogError(this, "did not generate a road route between doors");
			return;
		}
		auto lastOfSecondaryRouteStage = GetRouteById(2)[GetRouteById(2).Num() - 1];
		TArray<FVector> routeStage3;
		routeStage3.Emplace(lastOfSecondaryRouteStage);
		FindPrimaryRouteBetweenRecursive(routeStage3, GetRandomEndZoneLocation(), 70 + (doorLocs.Num() * 20), 3, false);
	}

	NewObject<URoadGenerations>(this)
		->SetRoadRoute(PrimaryRoute)
		->SetupGeneration(GetWorld(), RandomStream, ObstacleAllowedLocations)
		->Generate();
	UDebugMessages::LogDisplay(this, "GenericRoad gen #1");

	NewObject<URoadGenerations>(this)
		->SetRoadRoute(OffshotRoute1)
		->SetupGeneration(GetWorld(), RandomStream, ObstacleAllowedLocations)
		->Generate();
	UDebugMessages::LogDisplay(this, "GenericRoad gen #2");

	NewObject<URoadGenerations>(this)
		->SetRoadRoute(OffshotRoute2)
		->SetupGeneration(GetWorld(), RandomStream, ObstacleAllowedLocations)
		->Generate();
	UDebugMessages::LogDisplay(this, "GenericRoad gen #3");

	OffshotRoute4 = totalBuildingUsage;
	OffshotRoute5 = buildingToBuildingRoadGen->GetRoadRoute();
	auto route = GetRouteById(1);

	GenerateCaveWalls();
	ObstacleAllowedLocations = RemoveListFromList(ObstacleAllowedLocations, AllWallLocations);
	UDebugMessages::LogDisplay(this, "Cave Walls");

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
