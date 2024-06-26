// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseRunnable.h"
#include "LevelGenerationRunnable.generated.h"

/**
 * 
 */
UCLASS()
class SV_API ULevelGenerationRunnable : public UBaseRunnable
{
	GENERATED_BODY()
	
public:
	virtual void ActivateThread() override;
	ULevelGenerationRunnable* InsertVariables();

	void AddDebugActor(AActor* actor);
	UPROPERTY() TArray<AActor*> DebugActors;
private:

	void CreateGrid(int maxX, int maxY, int elevation = 1);
	void SetSpawnAndEndZone();

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

	void GenerateBoundaryWalls();

	void FindPrimaryRouteBetweenRecursive(TArray<FVector> currentRoute, FVector end, int maxRoute, int routeId);


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
};
