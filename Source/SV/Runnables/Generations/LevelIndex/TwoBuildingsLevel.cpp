// Fill out your copyright notice in the Description page of Project Settings.

#include "TwoBuildingsLevel.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../BuildingGeneration.h"
#include "../ExteriorGenerations.h"
#include "../MiscellaneousGenerations.h"
#include "../../../World/WorldGridItemActor.h"
#include "../RoadGenerations.h"
#include "../../../Utilities/SvUtilities.h"
#include "../../Generations/EnemyGeneration.h"
#include "../../Generations/PlayerGeneration.h"
#include "../../Generations/StaticSpawnerGeneration.h"
#include "../../Generations/MiscellaneousGenerations.h"
#include "../SpawnZoneGeneration.h"
#include "../EndZoneGeneration.h"
// #include "Algo/Filter.h"
#include "Algo/Transform.h"

#pragma optimize("", off)
void UTwoBuildingsLevel::GenerateLevel()
{
	int minX = 20;
	MaxX = 40;
	int minY = 20;
	MaxY = 40;

	int XBorder = RandomStream.RandRange(minX, MaxX);
	int YBorder = RandomStream.RandRange(minY, MaxY);

	// create full grid list of 0 to max of each border
	CreateGrid(MaxX, MaxY);
	// set spawn zone and completion zone
	SetSpawnAndEndZone();
	// generate boundary walls
	GenerateBoundaryWalls();

	// bool bDebug = false;
	// if (bDebug)
	// 	return UDebugMessages::LogError(this, "Manual return out of level gen, this is for debug only");

	TArray<FVector> spawnArr;
	spawnArr.Emplace(GetRandomStartZoneLocation());

	TArray<FVector> endArr;
	endArr.Emplace(GetRandomEndZoneLocation());

	auto world = GetWorld();

	int maxBuildingX = MaxX - 3;
	int maxBuildingY = MaxY - 3;

	for (int i = 3; i < maxBuildingX; i++)
		for (int j = 3; j < maxBuildingY; j++)
			ObstacleAllowedLocations.Emplace(FVector(i * 100, j * 100, 0));

	ObstacleAllowedLocations = RemoveListFromList(ObstacleAllowedLocations, SpawnZone);
	ObstacleAllowedLocations = RemoveListFromList(ObstacleAllowedLocations, EndZone);

	auto spawnZoneGen = NewObject<USpawnZoneGeneration>(this)
							->SetSpawnLocations(SpawnZone)
							->SetupGeneration(GetWorld(), RandomStream, ObstacleAllowedLocations)
							->SetStartAndEndZones(SpawnZone, EndZone)
							->Generate();

	auto endZoneGen = NewObject<UEndZoneGeneration>(this)
						  ->SetSpawnLocations(EndZone)
						  ->SetupGeneration(GetWorld(), RandomStream, ObstacleAllowedLocations)
						  ->SetStartAndEndZones(SpawnZone, EndZone)
						  ->Generate();
	UpdateLoadLevelWidget("Finding Route", .1f);
	GenerateBuildings();

	GenerateRoads();
	UpdateLoadLevelWidget("Obstacles", .2f);
	GenerateExteriors();

	GenerateCaveWalls();
	UpdateLoadLevelWidget("Structures", .3f);
	TArray<FGridDataItem *> onlyRoads;
	for (FGridDataItem &dataItem : GridData.GetGridItems())
	{
		if (dataItem.GetIsPotentialMiscItemSpace())
			onlyRoads.Emplace(&dataItem);
	}

	TArray<FVector> miscLocations;
	Algo::Transform(onlyRoads, miscLocations, [](FGridDataItem *Item)
					{ return Item->GetLocation(); });

	auto miscGens = NewObject<UMiscellaneousGenerations>(this)
						->SetupGeneration(GetWorld(), RandomStream, miscLocations)
						->Generate();

	SetLocationsAsDebris(miscGens->GetTotalUsedLocations());
	UpdateLoadLevelWidget("Roading", .4f);
	ObstacleAllowedLocations = RemoveListFromList(ObstacleAllowedLocations, miscGens->GetTotalUsedLocations());

	auto enemyGen = NewObject<UEnemyGeneration>(this)
						->SetPodAmountAndTier(5, 1)
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
	UpdateLoadLevelWidget("People", .5f);
	// DebugGenerations();
	FinishUp();
	UpdateLoadLevelWidget("Complete", 1);
}

void UTwoBuildingsLevel::GenerateBuildings()
{
	TArray<FVector> totalBuildingUsage;

	UDebugMessages::LogDisplay(this, "did basic generation");
	auto buildinGen = (UBuildingGeneration *)NewObject<UBuildingGeneration>(this)
						  ->SetAmountToGenerate(2)
						  ->SetupGeneration(GetWorld(), RandomStream, USvUtilities::RandomizeList(ObstacleAllowedLocations))
						  ->Generate();

	totalBuildingUsage = buildinGen->GetTotalUsedLocations();
	FindRouteIgnoreLocations = buildinGen->GetRequiredLocations();
	ObstacleAllowedLocations = RemoveListFromList(ObstacleAllowedLocations, totalBuildingUsage);

	auto usedBuffers = buildinGen->GetRequiredBufferLocations();

	SetLocationsAsPrefab(buildinGen->GetRequiredLocations());
	SetLocationsAsBuffer(usedBuffers);

	UDebugMessages::LogDisplay(this, "completed building gen");
	auto doorLocs = buildinGen->GetDoorLocations();
	if (doorLocs.Num() == 1)
	{

		auto allowedSmallerBuildingLocations = ObstacleAllowedLocations;
		allowedSmallerBuildingLocations = RemoveListFromList(allowedSmallerBuildingLocations, buildinGen->GetRequiredBufferLocations());

		UDebugMessages::LogWarning(this, "failed to generate 2 buildings, manually creating another");
		auto secondaryBuildingGen = (UBuildingGeneration *)NewObject<UBuildingGeneration>(this)
										->SetAmountToGenerate(1)
										->SetupGeneration(GetWorld(), RandomStream, USvUtilities::RandomizeList(allowedSmallerBuildingLocations))
										->Generate();

		doorLocs.Emplace(secondaryBuildingGen->GetDoorLocations()[0]);
		auto secondaryUsedLocs = secondaryBuildingGen->GetRequiredLocations();
		auto secondaryUsedBuffers = secondaryBuildingGen->GetRequiredBufferLocations();

		SetLocationsAsPrefab(secondaryUsedLocs);
		SetLocationsAsBuffer(secondaryUsedBuffers);

		FindRouteIgnoreLocations = CombineList(FindRouteIgnoreLocations, secondaryUsedLocs);
		ObstacleAllowedLocations = RemoveListFromList(ObstacleAllowedLocations, secondaryUsedLocs);
		totalBuildingUsage = CombineList(totalBuildingUsage, secondaryUsedLocs);
	}

	if (doorLocs.Num() > 1)
	{
		for (int i = 0; i < doorLocs.Num(); i++)
		{
			auto closestAllowedStartLoc = GetClosestAllowedLocation(doorLocs[i]);
			// SpawnDebugGrid_SetIsStart(closestAllowedStartLoc + FVector(0, 0, 50));

			if (i == 0)
			{
				// Get route from start to this Door
				TArray<FVector> primaryRouteStage;
				auto randomStart = GetClosestAllowedLocation(GetRandomStartZoneLocation());
				// SpawnDebugGrid_SetIsStart(randomStart + FVector(0, 0, 50));
				primaryRouteStage.Emplace(randomStart);
				UDebugMessages::LogDisplay(this, "finding route from " + randomStart.ToString() + " to: " + closestAllowedStartLoc.ToString());
				FindPrimaryRouteBetweenRecursive(primaryRouteStage, closestAllowedStartLoc, 70 + (i * 20), 1, false);
				UDebugMessages::LogDisplay(this, "did 0 iteration loop");
			}

			if (doorLocs.Num() > i + 1)
			{
				auto closestAllowedEndLoc = GetClosestAllowedLocation(doorLocs[i + 1]);

				auto lastOfPrimaryRouteStage = GetClosestAllowedLocation(PrimaryRoute[PrimaryRoute.Num() - 1]);
				TArray<FVector> routeStage2;
				routeStage2.Emplace(lastOfPrimaryRouteStage);

				UDebugMessages::LogDisplay(this, "finding route from " + lastOfPrimaryRouteStage.ToString() + " to: " + closestAllowedEndLoc.ToString());
				FindPrimaryRouteBetweenRecursive(routeStage2, closestAllowedEndLoc, 140 + (i * 20), 2, false);
				UDebugMessages::LogDisplay(this, "did stage 2 route loop");

				URoadGenerations *buildingToBuildingRoadGen =
					(URoadGenerations *)NewObject<URoadGenerations>(this)
						->SetRoadGenStartAndEndLocation(closestAllowedStartLoc, closestAllowedEndLoc)
						->SetupGeneration(GetWorld(), RandomStream, ObstacleAllowedLocations)
						->SetStartAndEndZones(SpawnZone, EndZone)
						->Generate();

				OffshootRoute5 = buildingToBuildingRoadGen->GetRoadRoute();
				SetLocationsAsRoad(OffshootRoute5);
				UDebugMessages::LogDisplay(this, "generated roads");
			}
		}

		if (GetRouteById(2).IsEmpty())
		{
			UDebugMessages::LogError(this, "did not generate a road route between doors");
			return;
		}
		auto lastOfSecondaryRouteStage = GetRouteById(2)[GetRouteById(2).Num() - 1];
		TArray<FVector> routeStage3;
		routeStage3.Emplace(lastOfSecondaryRouteStage);
		FindPrimaryRouteBetweenRecursive(routeStage3, GetRandomEndZoneLocation(), 70 + (doorLocs.Num() * 20), 3, false);
	}

	OffshootRoute4 = totalBuildingUsage;
}
void UTwoBuildingsLevel::GenerateRoads()
{
	auto roadRoute1 = (URoadGenerations *)NewObject<URoadGenerations>(this)
						  ->SetRoadRoute(PrimaryRoute)
						  ->SetupGeneration(GetWorld(), RandomStream, ObstacleAllowedLocations)
						  ->Generate();
	UDebugMessages::LogDisplay(this, "GenericRoad gen #1");

	auto roadRoute2 = (URoadGenerations *)NewObject<URoadGenerations>(this)
						  ->SetRoadRoute(OffshootRoute1)
						  ->SetupGeneration(GetWorld(), RandomStream, ObstacleAllowedLocations)
						  ->Generate();
	UDebugMessages::LogDisplay(this, "GenericRoad gen #2");

	auto roadRoute3 = (URoadGenerations *)NewObject<URoadGenerations>(this)
						  ->SetRoadRoute(OffshootRoute2)
						  ->SetupGeneration(GetWorld(), RandomStream, ObstacleAllowedLocations)
						  ->Generate();
	UDebugMessages::LogDisplay(this, "GenericRoad gen #3");

	SetLocationsAsRoad(roadRoute1->GetRoadRoute());
	SetLocationsAsRoad(roadRoute2->GetRoadRoute());
	SetLocationsAsRoad(roadRoute3->GetRoadRoute());
}

void UTwoBuildingsLevel::GenerateExteriors()
{
	auto exteriorGen = (UExteriorGenerations *)NewObject<UExteriorGenerations>(this)
						   ->SetAmountToGenerate(1)
						   ->SetupGeneration(GetWorld(), RandomStream, USvUtilities::RandomizeList(ObstacleAllowedLocations))
						   ->Generate();

	auto usedLocs = exteriorGen->GetRequiredLocations();
	auto usedBuffers = exteriorGen->GetRequiredBufferLocations();

	FindRouteIgnoreLocations = CombineList(FindRouteIgnoreLocations, usedLocs);

	auto allStartLocations = exteriorGen->GetStartLocations();
	for (int i = 0; i < allStartLocations.Num(); i++)
	{
		TArray<FVector> routeFinder;
		routeFinder.Emplace(allStartLocations[i]);
		FindPrimaryRouteBetweenRecursive(routeFinder, GetClosestAllowedLocation(GetRandomEndZoneLocation()), 180, 4, false);

		UDebugMessages::LogDisplay(this, "Route ToExterior " + FString::SanitizeFloat(routeFinder.Num()));

		auto roadRoute = (URoadGenerations *)NewObject<URoadGenerations>(this)
							 ->SetRoadRoute(OffshootRoute3)
							 //->SetRoadGenStartAndEndLocation(GetRandomEndZoneLocation(), allStartLocations[i])
							 ->SetupGeneration(GetWorld(), RandomStream, ObstacleAllowedLocations)
							 ->Generate();
		SetLocationsAsRoad(roadRoute->GetRoadRoute());
	}

	SetLocationsAsPrefab(usedLocs);
	SetLocationsAsBuffer(usedBuffers);
	ObstacleAllowedLocations = RemoveListFromList(ObstacleAllowedLocations, usedLocs);
	ObstacleAllowedLocations = RemoveListFromList(ObstacleAllowedLocations, usedBuffers);

	auto usedWithoutStartLocations = RemoveListFromList(usedLocs, allStartLocations);
	FindRouteIgnoreLocations = CombineList(FindRouteIgnoreLocations, usedWithoutStartLocations);
}
#pragma optimize("", on)