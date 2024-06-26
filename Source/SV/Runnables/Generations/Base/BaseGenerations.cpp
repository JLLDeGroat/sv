// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGenerations.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

#pragma optimize("", off)
UBaseGenerations::UBaseGenerations() {

}

UBaseGenerations* UBaseGenerations::SetupGeneration(UWorld* world, FRandomStream random, TArray<FVector> allowedSpots) {
	World = world;
	RandomStream = random;
	AllowedSpots = allowedSpots;
	return this;
}

UBaseGenerations* UBaseGenerations::Generate() {
	UDebugMessages::LogError(this, "GENERATION NOT WORKING, NO IMPLEMENTATION");
	return this;
}

void UBaseGenerations::SetChanceIncrement(int amount) {
	ChanceIncrement += amount;
}
void UBaseGenerations::SetChanceDecrement(int amount) {
	ChanceDecrement -= amount;
}

bool UBaseGenerations::ShouldGenerate() {
	return CurrentChance > RandomStream.RandRange(1, 1001);
}

void UBaseGenerations::SetRequiredSpots(TArray<FVector> requiredSpots) {
	RequiredSpots = requiredSpots;
}

bool UBaseGenerations::CanFitInLocation(FVector loc) {
	auto requiredLocs = CreateRequiredLocations(loc);

	for (int i = 0; i < requiredLocs.Num(); i++)
		if (!IsWithinAllowedList(requiredLocs[i]) ||
			IsWithinCurrentUsedList(requiredLocs[i]))
			return false;

	return true;
}

void UBaseGenerations::IncrementChance() {
	CurrentChance += ChanceIncrement;
}
void UBaseGenerations::DecrementChance() {
	CurrentChance -= ChanceDecrement;
}

TArray<FVector> UBaseGenerations::CreateRequiredLocations(FVector original) {
	TArray<FVector> locs;
	for (int i = 0; i < RequiredSpots.Num(); i++)
		locs.Emplace(original + RequiredSpots[i]);

	RequiredCalculatedSpots = locs;
	return locs;
}

void UBaseGenerations::AddToUsedSpots(TArray<FVector> locs) {
	for (int i = 0; i < locs.Num(); i++)
		AllUsedSpots.Emplace(locs[i]);
}


TArray<FVector> UBaseGenerations::GetUnusedSpotsLeft() {
	TArray<FVector> result;
	for (int i = 0; i < AllowedSpots.Num(); i++) {
		if (!IsWithinCurrentUsedList(AllowedSpots[i]))
			result.Emplace(AllowedSpots[i]);
	}
	return result;
}

UBaseGenerations* UBaseGenerations::SetRoadGenStartAndEndLocation(FVector start, FVector end) {
	StartLocation = start;
	EndLocation = end;

	return this;
}

UBaseGenerations* UBaseGenerations::SetStartAndEndZones(TArray<FVector> startZones, TArray<FVector> endZones) {
	StartZones = startZones;
	EndZones = endZones;
	return this;
}

bool UBaseGenerations::IsWithinAllowedList(FVector location) {
	for (int i = 0; i < AllowedSpots.Num(); i++)
		if (AllowedSpots[i] == location)
			return true;

	return false;
}

bool UBaseGenerations::IsWithinCurrentUsedList(FVector location) {
	for (int i = 0; i < AllUsedSpots.Num(); i++)
		if (AllUsedSpots[i] == location)
			return true;

	return false;
}


bool UBaseGenerations::IsRequiredSpotBottomLeft(FVector loc) {
	auto left = loc + FVector(-100, 0, 0);
	auto down = loc + FVector(0, -100, 0);

	for (int i = 0; i < RequiredCalculatedSpots.Num(); i++)
		if (RequiredCalculatedSpots[i] == left || RequiredCalculatedSpots[i] == down)
			return false;

	return true;
}
bool UBaseGenerations::IsRequiredSpotTopLeft(FVector loc) {
	auto right = loc + FVector(100, 0, 0);
	auto down = loc + FVector(0, -100, 0);

	for (int i = 0; i < RequiredCalculatedSpots.Num(); i++) {
		if (RequiredCalculatedSpots[i] == right || RequiredCalculatedSpots[i] == down)
			return false;
	}
	return true;
}
bool UBaseGenerations::IsRequiredSpotBottomRight(FVector loc) {
	auto left = loc + FVector(-100, 0, 0);
	auto up = loc + FVector(0, 100, 0);

	for (int i = 0; i < RequiredCalculatedSpots.Num(); i++) {
		if (RequiredCalculatedSpots[i] == left || RequiredCalculatedSpots[i] == up)
			return false;
	}

	return true;
}
bool UBaseGenerations::IsRequiredSpotTopRight(FVector loc) {
	auto right = loc + FVector(100, 0, 0);
	auto up = loc + FVector(0, 100, 0);

	for (int i = 0; i < RequiredCalculatedSpots.Num(); i++) {
		if (RequiredCalculatedSpots[i] == right || RequiredCalculatedSpots[i] == up)
			return false;
	}
	return true;
}

bool UBaseGenerations::IsRequiredSpotLeftWall(FVector loc) {
	auto left = loc + FVector(0, -100, 0);
	return !IsWithinRequiredSpots(left);
}
bool UBaseGenerations::IsRequiredSpotRightWall(FVector loc) {
	auto right = loc + FVector(0, 100, 0);
	return !IsWithinRequiredSpots(right);
}
bool UBaseGenerations::IsRequiredSpotTopWall(FVector loc) {
	auto up = loc + FVector(100, 0, 0);
	return !IsWithinRequiredSpots(up);
}
bool UBaseGenerations::IsRequiredSpotBottomWall(FVector loc) {
	auto down = loc + FVector(-100, 0, 0);
	return !IsWithinRequiredSpots(down);
}

bool UBaseGenerations::IsWithinRequiredSpots(FVector location) {
	for (int i = 0; i < RequiredCalculatedSpots.Num(); i++)
		if (RequiredCalculatedSpots[i] == location)
			return true;

	return false;
}

#pragma optimize("", on)