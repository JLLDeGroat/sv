// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseGenerations.generated.h"

/**
 *
 */
UCLASS()
class SV_API UBaseGenerations : public UObject
{
	GENERATED_BODY()

public:
	UBaseGenerations();

	UBaseGenerations *SetupGeneration(UWorld *world, FRandomStream random, TArray<FVector> allowedSpots);

	virtual UBaseGenerations *Generate();

	void IncrementChance();
	void DecrementChance();

	bool ShouldGenerate();

	void SetRequiredSpots(TArray<FVector> requiredSpots);

	bool CanFitInLocation(FVector loc);

	TArray<FVector> CreateRequiredLocations(FVector original);

	/*void AddToUsedSpots(TArray<FVector> locs); */

	TArray<FVector> GetUnusedSpotsLeft();
	// TArray<FVector> GetUsedSpots();

	UBaseGenerations *SetStartAndEndZones(TArray<FVector> startZones, TArray<FVector> endZones);

	void SetRequiredLocations(TArray<FVector> locations);
	void SetRequiredBufferLocations(TArray<FVector> locations);

	TArray<FVector> GetRequiredLocations();
	TArray<FVector> GetUsableLocations();
	TArray<FVector> GetRequiredBufferLocations();
	TArray<FVector> GetTotalUsedLocations();

protected:
	UPROPERTY()
	UWorld *World;
	/*UPROPERTY() TArray<FVector> AllowedSpots;
	UPROPERTY() TArray<FVector> RequiredSpots;
	UPROPERTY() TArray<FVector> RequiredCalculatedSpots;
	UPROPERTY() TArray<FVector> RequiredBufferSpots;
	UPROPERTY() TArray<FVector> RequiredCalculatedBufferSpots;
	UPROPERTY() TArray<FVector> AllUsedSpots;*/
	UPROPERTY()
	FRandomStream RandomStream;

	UPROPERTY()
	TArray<FVector> StartZones;
	UPROPERTY()
	TArray<FVector> EndZones;

	UPROPERTY()
	TArray<FVector> UsableLocations;
	UPROPERTY()
	TArray<FVector> TotalUsedLocations;

	// Locations that need to be added to offset, for multi block constructions
	UPROPERTY()
	TArray<FVector> TemplatedRequiredLocations;
	// locations used (not buffers) for each iteration
	UPROPERTY()
	TArray<FVector> IterationRequiredLocations;
	// locations used (not buffers) in total
	UPROPERTY()
	TArray<FVector> RequiredLocations;
	UPROPERTY()
	TArray<FVector> RequiredBufferLocations;

	// for road generators
	UPROPERTY()
	FVector StartLocation;
	UPROPERTY()
	FVector EndLocation;

	UPROPERTY()
	int MaxChange = 1000;
	UPROPERTY()
	int CurrentChance;
	UPROPERTY()
	int ChanceIncrement = 5;
	UPROPERTY()
	int ChanceDecrement = 500;

	void SetChanceDecrement(int amount);
	void SetChanceIncrement(int amount);

	bool IsRequiredSpotBottomLeft(FVector loc);
	bool IsRequiredSpotBottomRight(FVector loc);
	bool IsRequiredSpotTopLeft(FVector loc);
	bool IsRequiredSpotTopRight(FVector loc);

	bool IsRequiredSpotLeftWall(FVector loc);
	bool IsRequiredSpotRightWall(FVector loc);
	bool IsRequiredSpotTopWall(FVector loc);
	bool IsRequiredSpotBottomWall(FVector loc);

	void SpawnDebugGrid_SetIsStart(FVector location, float delay = .0001f);
	void SpawnDebugGrid_SetIsEnd(FVector location, float delay = .0001f);
	void SpawnDebugGrid_SetIsOffshoot(FVector location, float delay = .0001f);
	void SpawnDebugGrid_SetIsSpawn(FVector location, float delay = .0001f);
	void SpawnDebugGrid_SetIsObstacle(FVector location, float delay = .0001f);

	void SpawnDebugGrid_SetIsStart(TArray<FVector> locations, FVector offset = FVector(0, 0, -50), float delay = .0001f);
	void SpawnDebugGrid_SetIsEnd(TArray<FVector> locations, FVector offset = FVector(0, 0, -50), float delay = .0001f);
	void SpawnDebugGrid_SetIsOffshoot(TArray<FVector> locations, FVector offset = FVector(0, 0, -50), float delay = .0001f);
	void SpawnDebugGrid_SetIsSpawn(TArray<FVector> locations, FVector offset = FVector(0, 0, -50), float delay = .0001f);
	void SpawnDebugGrid_SetIsObstacle(TArray<FVector> locations, FVector offset = FVector(0, 0, -50), float delay = .0001f);

	TArray<FVector> GenerateBufferAroundLocations(TArray<FVector> locations, int buffer = 1);

	bool IsWithinList(TArray<FVector> list, FVector location);
	TArray<FVector> GetAdjacentsAroundLocation(FVector loc, int adjacentRadius);
	TArray<FVector> CombineList(TArray<FVector> locs1, TArray<FVector> locs2);
	FVector GetRandomLocationInList(TArray<FVector> locs);
	TArray<FVector> RemoveListFromList(TArray<FVector> bigList, TArray<FVector> removesThisListFromItemsFromBigList);

	UObject *LoadBlueprintActorInThread(FString reference, UObject *&obj, int secondsToWaitUntilGiveup = 5);

	void LogGenerationStart();
	void LogGenerationEnd();

private:
	/*bool IsWithinAllowedList(FVector location);
	bool IsWithinCurrentUsedList(FVector location);*/
	bool IsWithinRequiredSpots(FVector location);
	bool IsWithinRequiredBufferSpots(FVector location);
};
