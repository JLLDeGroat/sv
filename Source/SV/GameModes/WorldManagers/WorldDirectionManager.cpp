// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldDirectionManager.h"
#include "../../Utilities/SvUtilities.h"
#include "../../World/DirectionOptionActor.h"

// Sets default values for this component's properties
UWorldDirectionManager::UWorldDirectionManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UWorldDirectionManager::SetFullRouteList(TArray<FVector2D> arr) {
	FullRouteList = arr;
}

void UWorldDirectionManager::GenerateDirections(FVector2D currentLoc) {
	auto multiplier = USvUtilities::GetWorldMapGridMultiplier();
	FVector2D left;
	FVector2D right;
	FVector2D up;
	FVector2D down;
	if (!CanGoLeft(currentLoc, left))
	{
		auto gridLoc = FVector((left.Y * multiplier) + 50, (left.X * multiplier) + 50, 0);
		auto actor = GetWorld()->SpawnActor<ADirectionOptionActor>(gridLoc, FRotator(0, 90, 0));
		DirectionActors.Emplace(actor);
	}
	if (!CanGoRight(currentLoc, right))
	{
		auto gridLoc = FVector((right.Y * multiplier) + 50, (right.X * multiplier) + 50, 0);
		auto actor = GetWorld()->SpawnActor<ADirectionOptionActor>(gridLoc, FRotator(0, -90, 0));
		DirectionActors.Emplace(actor);
	}
	if (!CanGoUp(currentLoc, up))
	{
		auto gridLoc = FVector((up.Y * multiplier) + 50, (up.X * multiplier) + 50, 0);
		auto actor = GetWorld()->SpawnActor<ADirectionOptionActor>(gridLoc, FRotator(0, 180, 0));
		DirectionActors.Emplace(actor);
	}
	if (!CanGoDown(currentLoc, down))
	{
		auto gridLoc = FVector((down.Y * multiplier) + 50, (down.X * multiplier) + 50, 0);
		auto actor = GetWorld()->SpawnActor<ADirectionOptionActor>(gridLoc, FRotator(0));
		DirectionActors.Emplace(actor);
	}
}


bool UWorldDirectionManager::CanGoLeft(FVector2D loc, FVector2D& leftLoc) { // left is down
	leftLoc = GetLeft(loc);
	return !IsWithinRoute(leftLoc);
}
bool UWorldDirectionManager::CanGoRight(FVector2D loc, FVector2D& rightLoc) { // up is right
	rightLoc = GetRight(loc);
	return !IsWithinRoute(rightLoc);
}
bool UWorldDirectionManager::CanGoUp(FVector2D loc, FVector2D& upLoc) {
	upLoc = GetUp(loc);
	return !IsWithinRoute(upLoc);
}
bool UWorldDirectionManager::CanGoDown(FVector2D loc, FVector2D& downLoc) {
	downLoc = GetDown(loc);
	return !IsWithinRoute(downLoc);
}

FVector2D UWorldDirectionManager::GetLeft(FVector2D loc) {
	return loc - FVector2D(1, 0);
}
FVector2D UWorldDirectionManager::GetRight(FVector2D loc) {
	return loc + FVector2D(1, 0);
}
FVector2D UWorldDirectionManager::GetUp(FVector2D loc) {
	return loc + FVector2D(0, 1);
}
FVector2D UWorldDirectionManager::GetDown(FVector2D loc) {
	return loc - FVector2D(0, 1);
}

bool UWorldDirectionManager::IsWithinRoute(FVector2D loc) {
	for (int i = 0; i < FullRouteList.Num(); i++)
		if (loc == FullRouteList[i])
			return true;

	return false;
}

void UWorldDirectionManager::ClearDirections() {
	for (int i = 0; i < DirectionActors.Num(); i++)
		if (DirectionActors[i])
			DirectionActors[i]->Destroy();
	
	DirectionActors.Empty();
}