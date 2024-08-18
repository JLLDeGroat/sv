// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseGenerations.h"
#include "BuildingInteriorGeneration.generated.h"

/**
 *
 */
UCLASS()
class SV_API UBuildingInteriorGeneration : public UBaseGenerations
{
	GENERATED_BODY()

public:
	UBuildingInteriorGeneration();

	virtual UBaseGenerations* Generate() override;

	UBuildingInteriorGeneration* SetDoorLocations(TArray<FVector> locations);
	UBuildingInteriorGeneration* SetDoorLocation(FVector location);

	UBuildingInteriorGeneration* SetInternalRouteAmount(int amount);

protected:

	UPROPERTY() TArray<FVector> DoorLocations;

	void FindPrimaryRouteBetweenRecursive(TArray<FVector> currentRoute, FVector end, int maxRoute);

	bool CanReachDestination(FVector location, FVector end, int steps);
	bool IsFurtherFromEnd(FVector last, FVector current, FVector end);
	TArray<FVector> GetAdjacentGridItems(FVector item);
private:

	TArray<FVector> GenerateInternalRoute();
	UPROPERTY()int TotalIterations;
	UPROPERTY()int MaxTotalIteraions;
	UPROPERTY() int InternalRouteAmounts;

	UPROPERTY() TArray<FVector> InteriorRoute;
	UPROPERTY() TArray<FVector> ValidInteriorLocations;

	bool GenerateLongTable(FVector desiredLocation);
	bool GenerateItemInBuilding(FVector desiredLocation);

	TArray<FVector> GetLongTableTemplatedLocations(bool bShouldRotate);
	TArray<FVector> GetRollingTableTemplatedLocations(bool bShouldRotate);
	TArray<FVector> GetCornerTableTemplatedLocations(bool bShouldRotate);

	TArray<FVector> DetermineCorners();

};
