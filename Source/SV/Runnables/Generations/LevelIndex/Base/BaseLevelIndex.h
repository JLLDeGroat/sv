// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseLevelIndex.generated.h"

class UWorld;
/**
 *
 */
UCLASS()
class SV_API UBaseLevelIndex : public UObject
{
	GENERATED_BODY()

public:

	UBaseLevelIndex();

	UBaseLevelIndex* Setup(UWorld* world, FRandomStream randomStream);
	virtual void GenerateLevel();
	void KillObject();

protected:

	UWorld* GetWorld() const;
	UPROPERTY() UWorld* World;

	void AddDebugActor(AActor* actor);
	UPROPERTY() TArray<AActor*> DebugActors;

	void CreateGrid(int maxX, int maxY, int elevation = 1);
	void SetSpawnAndEndZone();

	UPROPERTY() FRandomStream RandomStream;
	UPROPERTY() int MaxX;
	UPROPERTY() int MaxY;

	UPROPERTY() TArray<FVector> Grid;
	UPROPERTY() TArray<FVector2D> TwoDGrid;

	UPROPERTY() TArray<FVector> SpawnZone;
	UPROPERTY() TArray<FVector> EndZone;

	UPROPERTY() TArray<FVector> PrimaryRoute;

	UPROPERTY() TArray<FVector> OffshotRoute1;
	UPROPERTY() TArray<FVector> OffshotRoute2;
	UPROPERTY() TArray<FVector> OffshotRoute3;
	UPROPERTY() TArray<FVector> OffshotRoute4;
	UPROPERTY() TArray<FVector> OffshotRoute5;

	UPROPERTY() TArray<FVector> AllWallLocations;
	UPROPERTY() TArray<FVector> WallLocations;

	UPROPERTY() TArray<FVector> ObstacleAllowedLocations;
	UPROPERTY() TArray<FVector> PotentialEnemyLocations;

	UPROPERTY() TArray<FVector> FindRouteIgnoreLocations;

	void GenerateBoundaryWalls();

	void FindPrimaryRouteBetweenRecursive(TArray<FVector> currentRoute, FVector end, int maxRoute, int routeId, bool bDebug = false);


	TArray<FVector> GetAdjacentGridItems(FVector item);
	bool IsAValidRouteItem(FVector item);
	bool IsWithinList(TArray<FVector> locations, FVector loc);
	bool IsAnyWithinList(TArray<FVector> locations, TArray<FVector> testLocs);
	bool CanReachDestination(FVector locations, FVector end, int steps);
	bool IsFurtherFromEnd(FVector last, FVector current, FVector end);

	void SetRouteById(int routeId, TArray<FVector> route);
	TArray<FVector> GetRouteById(int routeId);
	TArray<FVector> GetRandomLocationAlongPrimaryRoute();
	FVector GetRandomLocationWithinGrid();
	TArray<FVector> GetTotalRoutesList();

	void GenerateCaveWalls();
	bool CaveWallIsFullyEnclosed(FVector wall, TArray<FVector> allWalls);

	void FillAllObstacleAllowedLocations();

	FVector GetRandomStartZoneLocation();
	FVector GetRandomEndZoneLocation();
	FVector GetRandomLocationInList(TArray<FVector> locs);

	FVector GetClosestAllowedLocation(FVector loc);

	void SpawnDebugGrid_SetIsStart(FVector location, float delay = 0.001f);
	void SpawnDebugGrid_SetIsEnd(FVector location, float delay = 0.001f);
	void SpawnDebugGrid_SetIsOffshoot(FVector location, float delay = 0.001f);
	void SpawnDebugGrid_SetIsSpawn(FVector location, float delay = 0.001f);
	void SpawnDebugGrid_SetIsObstacle(FVector location, float delay = 0.001f);

	TArray<FVector> RemoveListFromList(TArray<FVector> bigList, TArray<FVector> removesThisListFromItemsFromBigList);
	TArray<FVector> CombineList(TArray<FVector> locs1, TArray<FVector> locs2);
private:

	UPROPERTY() bool bIsAlive;

};