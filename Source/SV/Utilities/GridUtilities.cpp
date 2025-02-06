// Fill out your copyright notice in the Description page of Project Settings.


#include "GridUtilities.h"
#include "SvUtilities.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

FVector UGridUtilities::GetNormalisedGridLocation(FVector location) {
	return FVector(NormalizeGridValue(location.X), NormalizeGridValue(location.Y), location.Z);
}

bool UGridUtilities::GetLocationsAtSameGrid(FVector vector1, FVector vector2) {
	if (vector1.X == vector2.X && vector1.Y == vector2.Y) {
		auto testZ1 = FVector(0, 0, vector1.Z);
		auto testZ2 = FVector(0, 0, vector2.Z);

		if (FVector::Dist(testZ1, testZ2) < 150) {
			return true;
		}
	}
	return false;
}

TArray<FVector> UGridUtilities::GetGridLocationSteps(FVector start, FVector end) {
	auto distance = end - start;
	auto gridGape = USvUtilities::GetGridGape();

	auto XSteps = distance.X / gridGape;
	auto YSteps = distance.Y / gridGape;

	if (XSteps < 0) XSteps *= -1;
	if (YSteps < 0) YSteps *= -1;

	TArray<FVector> locs;
	locs.Emplace(start);

	for (int i = 0; i < 1000; i++) {
		auto thisStep = locs[locs.Num() - 1];

		if (XSteps > 0) thisStep.X += distance.X > 0 ? gridGape : -gridGape;
		if (YSteps > 0) thisStep.Y += distance.Y > 0 ? gridGape : -gridGape;

		XSteps--;
		YSteps--;

		locs.Emplace(thisStep);

		if (XSteps <= 0 && YSteps <= 0)
			break;
	}

	//remove initial entry
	locs.RemoveAt(0);

	return locs;
}

FRotator UGridUtilities::FindLookAtRotation(FVector source, FVector target) {
	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(source, target);
	return rotation;
}


float UGridUtilities::NormalizeGridValue(float loc) {
	auto gridGape = USvUtilities::GetGridGape();
	int value = loc / gridGape;
	float remained = loc - (value * gridGape);

	if (remained > (gridGape / 2)) value++;

	return value * gridGape;
}

FVector UGridUtilities::GetRouteLocationAs3DLoc(FVector2D loc, bool bAddOffset) {
	auto multiplier = USvUtilities::GetWorldMapGridMultiplier();
	if (bAddOffset) return FVector((loc.Y * multiplier) + 50, (loc.X * multiplier) + 50, 0);
	else return FVector(loc.Y * multiplier, loc.X * multiplier, 0);
}
FVector2D UGridUtilities::GetRouteLocationFrom3DLoc(FVector loc, bool bRemoveOffset) {
	auto multiplier = USvUtilities::GetWorldMapGridMultiplier();
	if (bRemoveOffset)
		return FVector2D((loc.Y - 50) / multiplier, (loc.X - 50) / multiplier);
	else
		return FVector2D((loc.Y) / multiplier, (loc.X) / multiplier);
}

TArray<FVector> UGridUtilities::GetAllLocationsWithinRadius(FVector location, int radius) {
	auto radiusReal = radius * 100;

	TArray<FVector> locations;

	for (int x = -radiusReal; x < radiusReal; x += 100) {
		for (int y = -radiusReal; y < radiusReal; y += 100) {
			locations.Add(location + FVector(x, y, 0));
			//y + 100;
		}
		//x + 100;
	}

	return locations;
}