// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseGenerations.h"
#include "RoadGenerations.generated.h"

/**
 *
 */
UCLASS()
class SV_API URoadGenerations : public UBaseGenerations
{
	GENERATED_BODY()
public:
	URoadGenerations();

	virtual UBaseGenerations *Generate() override;

	TArray<FVector> GetRoadRoute();
	URoadGenerations *SetRoadGenStartAndEndLocation(FVector start, FVector end);
	URoadGenerations *SetRoadRoute(TArray<FVector> locations);
	URoadGenerations *SetRoadMaxIterations(int amount);

protected:
	TArray<FVector> GetAdjacentGridItems(FVector item);
	void FindPrimaryRouteBetweenRecursive(TArray<FVector> currentRoute, FVector end, int maxRoute);

	void PlaceRoad(FVector loc);

	UPROPERTY()
	TArray<FVector> Route;

	UPROPERTY()
	TArray<FVector> RoadAllowedList;

private:
	bool IsWithinList(TArray<FVector> locations, FVector loc);
	bool CanReachDestination(FVector location, FVector end, int steps);
	bool IsFurtherFromEnd(FVector last, FVector current, FVector end);

	UPROPERTY()
	int TotalIterations;
	UPROPERTY()
	int MaxTotalIteraions;
};