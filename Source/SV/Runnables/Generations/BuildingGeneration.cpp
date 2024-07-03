// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingGeneration.h"
#include "../../World/WorldGridItemActor.h"
#include "../../Environment/Constructions/Wall.h"
#include "../../Environment/Constructions/Door.h"
#include "../../Environment/Constructions/WallWindow.h"

#pragma optimize("", off)
UBaseGenerations* UBuildingGeneration::Generate() {
	SetRequiredSpots(GetBuildingRequiredLocations());
	for (int i = 0; i < AllowedSpots.Num(); i++) {
		if (CanFitInLocation(AllowedSpots[i])) {
			if (ShouldGenerate()) {
				DecrementChance();

				bool generatedDoor = false;

				auto requiredLocs = CreateRequiredLocations(AllowedSpots[i]);
				AddToUsedSpots(requiredLocs);

				auto world = GetWorld();
				/*FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, requiredLocs] {
					for (int i = 00; i < requiredLocs.Num(); i++) {
						auto actor = world->SpawnActor<AWorldGridItemActor>(requiredLocs[i], FRotator::ZeroRotator);
						actor->SetIsStart();
					}}, TStatId(), nullptr, ENamedThreads::GameThread);*/

				bool bgeneratedLeftWindows = false;
				bool bgeneratedRighttWindows = false;
				bool bgeneratedUpWindows = false;
				bool bgeneratedDownWindows = false;


				for (int x = 0; x < requiredLocs.Num(); x++) {
					if (IsRequiredSpotBottomRight(requiredLocs[x])) {
						BuildWall(requiredLocs[x] + FVector(50, 50, 0), FRotator(0, 180, 0));
						BuildWall(requiredLocs[x] + FVector(50, 150, 0), FRotator(0, 90, 0));
					}
					else if (IsRequiredSpotBottomLeft(requiredLocs[x])) {
						BuildWall(requiredLocs[x] + FVector(50, 50, 0), FRotator(0, 180, 0));
						BuildWall(requiredLocs[x] + FVector(50, 50, 0), FRotator(0, 90, 0));
					}
					else if (IsRequiredSpotTopRight(requiredLocs[x])) {
						BuildWall(requiredLocs[x] + FVector(50, 150, 0), FRotator(0, 90, 0));
						BuildWall(requiredLocs[x] + FVector(150, 150, 0));
					}
					else if (IsRequiredSpotTopLeft(requiredLocs[x])) {
						BuildWall(requiredLocs[x] + FVector(150, 50, 0), FRotator(0, 180, 0));
						BuildWall(requiredLocs[x] + FVector(50, 50, 0), FRotator(0, 90, 0));
					}
					else {

						if (IsRequiredSpotLeftWall(requiredLocs[x])) {
							if (!generatedDoor) {
								BuildDoor(requiredLocs[x] + FVector(50, 50, 0), FRotator(0, 90, 0));
								generatedDoor = true;
							}
							else {
								if (!bgeneratedLeftWindows) {
									if (!ShouldBuildWindow()) {
										BuildWall(requiredLocs[x] + FVector(50, 50, 0), FRotator(0, 90, 0));
										bgeneratedLeftWindows = true;
									}
									else BuildWindow(requiredLocs[x] + FVector(50, 50, 0), FRotator(0, 90, 0));
								}
								else BuildWall(requiredLocs[x] + FVector(50, 50, 0), FRotator(0, 90, 0));

							}
						}
						else if (IsRequiredSpotRightWall(requiredLocs[x])) {
							if (!generatedDoor) {
								BuildDoor(requiredLocs[x] + FVector(50, 150, 0), FRotator(0, 90, 0));
								generatedDoor = true;
							}
							else {
								if (!bgeneratedRighttWindows) {
									if (!ShouldBuildWindow()) {
										BuildWall(requiredLocs[x] + FVector(50, 150, 0), FRotator(0, 90, 0));
										bgeneratedRighttWindows = true;
									}
									else BuildWindow(requiredLocs[x] + FVector(50, 150, 0), FRotator(0, 90, 0));
								}
								else BuildWall(requiredLocs[x] + FVector(50, 150, 0), FRotator(0, 90, 0));

							}
						}
						else if (IsRequiredSpotBottomWall(requiredLocs[x])) {
							if (!generatedDoor) {
								BuildDoor(requiredLocs[x] + FVector(50, 50, 0), FRotator(0, 180, 0));
								generatedDoor = true;
							}
							else {
								if (!bgeneratedDownWindows) {
									if (!ShouldBuildWindow()) {
										BuildWall(requiredLocs[x] + FVector(50, 50, 0), FRotator(0, 180, 0));
										bgeneratedDownWindows = true;
									}
									else BuildWindow(requiredLocs[x] + FVector(50, 50, 0), FRotator(0, 180, 0));
								}
								else BuildWall(requiredLocs[x] + FVector(50, 50, 0), FRotator(0, 180, 0));

							}
						}
						else if (IsRequiredSpotTopWall(requiredLocs[x])) {
							if (!generatedDoor) {
								BuildDoor(requiredLocs[x] + FVector(150, 50, 0), FRotator(0, 180, 0));
								generatedDoor = true;
							}
							else {
								if (!bgeneratedUpWindows) {
									if (!ShouldBuildWindow()) {
										BuildWall(requiredLocs[x] + FVector(150, 50, 0), FRotator(0, 180, 0));
										bgeneratedUpWindows = true;
									}
									else BuildWindow(requiredLocs[x] + FVector(150, 50, 0), FRotator(0, 180, 0));
								}
								else BuildWall(requiredLocs[x] + FVector(150, 50, 0), FRotator(0, 180, 0));
							}
						}
					}
				}
			}
			else {
				//reset requiredlocations for new house size
				SetRequiredSpots(GetBuildingRequiredLocations());
				IncrementChance();
			}
		}
	}
	return this;
}

TArray<FVector> UBuildingGeneration::GetBuildingRequiredLocations() {

	TArray<FVector> result;

	auto random = RandomStream.RandRange(1, 1001);

	auto randomLength = RandomStream.RandRange(3, 8);
	auto randomWidth = RandomStream.RandRange(3, 4);

	if (random > 500) {
		for (int i = 0; i < randomLength; i++) {
			for (int x = 0; x < randomWidth; x++) {
				result.Emplace(FVector(i * 100, x * 100, 0));
			}
		}
	}
	else {
		for (int i = 0; i < randomLength; i++) {
			for (int x = 0; x < randomWidth; x++) {
				result.Emplace(FVector(x * 100, i * 100, 0));
			}
		}
	}
	return result;
}

void UBuildingGeneration::BuildWall(FVector loc, FRotator rot) {
	auto world = GetWorld();
	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, loc, rot] {
		auto actor = world->SpawnActor<AWall>(loc, rot);
		}, TStatId(), nullptr, ENamedThreads::GameThread);
}

void UBuildingGeneration::BuildDoor(FVector loc, FRotator rot) {
	auto world = GetWorld();
	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, loc, rot] {
		auto actor = world->SpawnActor<ADoor>(loc, rot);
		}, TStatId(), nullptr, ENamedThreads::GameThread);
}
void UBuildingGeneration::BuildWindow(FVector loc, FRotator rot) {
	auto world = GetWorld();
	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, loc, rot] {
		auto actor = world->SpawnActor<AWallWindow>(loc, rot);
		}, TStatId(), nullptr, ENamedThreads::GameThread);
}

bool UBuildingGeneration::ShouldBuildWindow() {
	auto num = RandomStream.RandRange(1, 1001);
	return num > 400;
}
#pragma optimize("", on)