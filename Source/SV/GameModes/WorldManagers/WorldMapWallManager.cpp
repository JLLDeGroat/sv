// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldMapWallManager.h"
#include "../../Environment/Constructions/Wall.h"
#include "../../Utilities/SvUtilities.h"

UWorldMapWallManager::UWorldMapWallManager(const FObjectInitializer& ObjectInitializer)
	: UActorComponent(ObjectInitializer)
{
	Vector2DTo3DMultiplier = USvUtilities::GetWorldMapGridMultiplier();
}

void UWorldMapWallManager::SetFullRouteList(TArray<FVector2D> route) {
	FullRouteList = route;
}
TArray<FVector2D> UWorldMapWallManager::GetFullRouteList() {
	return FullRouteList;
}
#pragma optimize("", off)
void UWorldMapWallManager::GenerateWalls() {
	auto multipler = USvUtilities::GetWorldMapGridMultiplier();

	for (int i = 0; i < FullRouteList.Num(); i++) {
		FVector spawnLoc;
		FRotator spawnRot;

		if (ShouldBuildWallToLeft(FullRouteList[i], spawnLoc, spawnRot)) {
			GetWorld()->SpawnActor<AWall>(spawnLoc, spawnRot);
		}
		if (ShouldBuildWallToRight(FullRouteList[i], spawnLoc, spawnRot)) {
			GetWorld()->SpawnActor<AWall>(spawnLoc, spawnRot);
		}
		if (ShouldBuildWallToUp(FullRouteList[i], spawnLoc, spawnRot)) {
			GetWorld()->SpawnActor<AWall>(spawnLoc, spawnRot);
		}
		if (ShouldBuildWallToBottom(FullRouteList[i], spawnLoc, spawnRot)) {
			GetWorld()->SpawnActor<AWall>(spawnLoc, spawnRot);
		}
	}
}

bool UWorldMapWallManager::ShouldBuildWallToLeft(FVector2D loc, FVector& outLoc, FRotator& outRot) {
	auto newLoc = loc - FVector2D(1, 0);
	outRot = FRotator(0, 90, 0);
	outLoc = Make3DLocation(loc);
	return !IsWithinRoute(newLoc);
}
bool UWorldMapWallManager::ShouldBuildWallToRight(FVector2D loc, FVector& outLoc, FRotator& outRot) {
	auto newLoc = loc + FVector2D(1, 0);
	outRot = FRotator(0, 90, 0);
	outLoc = Make3DLocation(loc);
	outLoc.Y += 1 * Vector2DTo3DMultiplier;
	return !IsWithinRoute(newLoc);
}
bool UWorldMapWallManager::ShouldBuildWallToUp(FVector2D loc, FVector& outLoc, FRotator& outRot) {
	auto newLoc = loc + FVector2D(0, 1);
	outLoc = Make3DLocation(loc);
	outRot = FRotator(0, 180, 0);
	outLoc.X += 1 * Vector2DTo3DMultiplier;
	return !IsWithinRoute(newLoc);
}
bool UWorldMapWallManager::ShouldBuildWallToBottom(FVector2D loc, FVector& outLoc, FRotator& outRot) {
	auto newLoc = loc - FVector2D(0, 1);
	outLoc = Make3DLocation(loc);
	outRot = FRotator(0, 180, 0);
	return !IsWithinRoute(newLoc);
}


bool UWorldMapWallManager::CanGoLeft(FVector2D loc, FVector2D& leftLoc) { // left is down
	leftLoc = GetLeft(loc);
	return !IsWithinRoute(leftLoc);
}
bool UWorldMapWallManager::CanGoRight(FVector2D loc, FVector2D& rightLoc) { // up is right
	rightLoc = GetRight(loc);
	return !IsWithinRoute(rightLoc);
}
bool UWorldMapWallManager::CanGoUp(FVector2D loc, FVector2D& upLoc) {
	upLoc = GetUp(loc);
	return !IsWithinRoute(upLoc);
}
bool UWorldMapWallManager::CanGoDown(FVector2D loc, FVector2D& downLoc) {
	downLoc = GetDown(loc);
	return !IsWithinRoute(downLoc);
}

FVector2D UWorldMapWallManager::GetLeft(FVector2D loc) {
	return loc - FVector2D(1, 0);
}
FVector2D UWorldMapWallManager::GetRight(FVector2D loc) {
	return loc + FVector2D(1, 0);
}
FVector2D UWorldMapWallManager::GetUp(FVector2D loc) {
	return loc + FVector2D(0, 1);
}
FVector2D UWorldMapWallManager::GetDown(FVector2D loc) {
	return loc - FVector2D(0, 1);
}

bool UWorldMapWallManager::IsWithinRoute(FVector2D loc) {
	for (int i = 0; i < FullRouteList.Num(); i++)
		if (loc == FullRouteList[i])
			return true;

	return false;
}

FVector UWorldMapWallManager::Make3DLocation(FVector2D loc) {
	return FVector(loc.Y * Vector2DTo3DMultiplier, loc.X * Vector2DTo3DMultiplier, 0);
}
#pragma optimize("", on)
