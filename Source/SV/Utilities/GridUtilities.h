// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridUtilities.generated.h"
/**
 * 
 */


UCLASS()
class SV_API UGridUtilities : public UObject
{
	GENERATED_BODY()
public:
	static FVector GetNormalisedGridLocation(FVector location);
	static bool GetLocationsAtSameGrid(FVector vector1, FVector vector2);
	static TArray<FVector> GetGridLocationSteps(FVector start, FVector end);

	static FRotator FindLookAtRotation(FVector source, FVector target);

	static FVector GetRouteLocationAs3DLoc(FVector2D loc, bool bAddOffset = false);
	static FVector2D GetRouteLocationFrom3DLoc(FVector loc, bool bRemoveOffset = false);

	static TArray<FVector> GetAllLocationsWithinRadius(FVector location, int radius);

private:

	static float NormalizeGridValue(float loc);
};
