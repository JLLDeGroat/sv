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

	UBaseGenerations* SetupGeneration(UWorld* world, FRandomStream random, TArray<FVector> allowedSpots);

	virtual UBaseGenerations* Generate();

	void IncrementChance();
	void DecrementChance();

	bool ShouldGenerate();

	void SetRequiredSpots(TArray<FVector> requiredSpots);

	bool CanFitInLocation(FVector loc);

	TArray<FVector> CreateRequiredLocations(FVector original);

	void AddToUsedSpots(TArray<FVector> locs);

	TArray<FVector> GetUnusedSpotsLeft();

	UBaseGenerations* SetRoadGenStartAndEndLocation(FVector start, FVector end);
	UBaseGenerations* SetStartAndEndZones(TArray<FVector> startZones, TArray<FVector> endZones);
protected:

	UPROPERTY() UWorld* World;
	UPROPERTY() TArray<FVector> AllowedSpots;
	UPROPERTY() TArray<FVector> RequiredSpots;
	UPROPERTY() TArray<FVector> RequiredCalculatedSpots;
	UPROPERTY() TArray<FVector> AllUsedSpots;
	UPROPERTY() FRandomStream RandomStream;

	UPROPERTY() TArray<FVector> StartZones;
	UPROPERTY() TArray<FVector> EndZones;

	//for road generators
	UPROPERTY() FVector StartLocation;
	UPROPERTY() FVector EndLocation;

	UPROPERTY() int MaxChange = 1000;
	UPROPERTY() int CurrentChance;
	UPROPERTY() int ChanceIncrement = 5;
	UPROPERTY() int ChanceDecrement = 500;

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


private:

	bool IsWithinAllowedList(FVector location);
	bool IsWithinCurrentUsedList(FVector location);
	bool IsWithinRequiredSpots(FVector location);
};
