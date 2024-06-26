// Fill out your copyright notice in the Description page of Project Settings.


#include "FenceGenerations.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Environment/Constructions/Fence.h"
#include "../../Environment/Natural/SoilPlot.h"
#include "../../World/WorldGridItemActor.h"

#pragma optimize("", off)
UBaseGenerations* UFenceGenerations::Generate() {
	SetRequiredSpots(GenerateRequiredLocations());

	for (int i = 0; i < AllowedSpots.Num(); i++) {
		if (CanFitInLocation(AllowedSpots[i])) {
			if (ShouldGenerate()) {
				DecrementChance();
				auto requiredLocs = CreateRequiredLocations(AllowedSpots[i]);
				AddToUsedSpots(requiredLocs);

				/*auto world = World;
				FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, requiredLocs] {
					for (int i = 00; i < requiredLocs.Num(); i++) {
						auto actor = world->SpawnActor<AWorldGridItemActor>(requiredLocs[i], FRotator::ZeroRotator);
						actor->SetIsObstacle();
					}}, TStatId(), nullptr, ENamedThreads::GameThread);*/


				for (int x = 0; x < requiredLocs.Num(); x++) {
					if (IsRequiredSpotBottomRight(requiredLocs[x])) {
						BuildFence(requiredLocs[x] + FVector(50, 50, 0), FRotator(0, 180, 0));
						BuildFence(requiredLocs[x] + FVector(50, 150, 0), FRotator(0, 90, 0));
					}
					else if (IsRequiredSpotBottomLeft(requiredLocs[x])) {
						BuildFence(requiredLocs[x] + FVector(50, 50, 0), FRotator(0, 180, 0));
						BuildFence(requiredLocs[x] + FVector(50, 50, 0), FRotator(0, 90, 0));
					}
					else if (IsRequiredSpotTopRight(requiredLocs[x])) {
						BuildFence(requiredLocs[x] + FVector(50, 150, 0), FRotator(0, 90, 0));
						BuildFence(requiredLocs[x] + FVector(150, 150, 0));
					}
					else if (IsRequiredSpotTopLeft(requiredLocs[x])) {
						BuildFence(requiredLocs[x] + FVector(150, 50, 0), FRotator(0, 180, 0));
						BuildFence(requiredLocs[x] + FVector(50, 50, 0), FRotator(0, 90, 0));
					}
					else {

						if (IsRequiredSpotLeftWall(requiredLocs[x])) {
							BuildFence(requiredLocs[x] + FVector(50, 50, 0), FRotator(0, 90, 0));
						}
						else if (IsRequiredSpotRightWall(requiredLocs[x])) {
							BuildFence(requiredLocs[x] + FVector(50, 150, 0), FRotator(0, 90, 0));
						}
						else if (IsRequiredSpotBottomWall(requiredLocs[x])) {
							BuildFence(requiredLocs[x] + FVector(50, 50, 0), FRotator(0, 180, 0));
						}
						else if (IsRequiredSpotTopWall(requiredLocs[x])) {
							BuildFence(requiredLocs[x] + FVector(150, 50, 0), FRotator(0, 180, 0));
						}
						else {
							auto num = RandomStream.RandRange(1, 1001);
							if (num > 500)
								BuildPlot(requiredLocs[x]);
						}
					}
				}
			}
			else IncrementChance();
		}
	}
	return this;
}

TArray<FVector> UFenceGenerations::GenerateRequiredLocations() {
	TArray<FVector> requiredLocations;

	requiredLocations.Emplace(FVector(0, 0, 0));
	requiredLocations.Emplace(FVector(100, 0, 0));
	requiredLocations.Emplace(FVector(200, 0, 0));
	requiredLocations.Emplace(FVector(300, 0, 0));
	requiredLocations.Emplace(FVector(400, 0, 0));

	requiredLocations.Emplace(FVector(0, 100, 0));
	requiredLocations.Emplace(FVector(0, 200, 0));
	requiredLocations.Emplace(FVector(0, 300, 0));
	requiredLocations.Emplace(FVector(0, 400, 0));

	requiredLocations.Emplace(FVector(100, 100, 0));
	requiredLocations.Emplace(FVector(200, 100, 0));
	requiredLocations.Emplace(FVector(300, 100, 0));
	requiredLocations.Emplace(FVector(400, 100, 0));

	requiredLocations.Emplace(FVector(100, 200, 0));
	requiredLocations.Emplace(FVector(200, 200, 0));
	requiredLocations.Emplace(FVector(300, 200, 0));
	requiredLocations.Emplace(FVector(400, 200, 0));

	requiredLocations.Emplace(FVector(100, 300, 0));
	requiredLocations.Emplace(FVector(200, 300, 0));
	requiredLocations.Emplace(FVector(300, 300, 0));
	requiredLocations.Emplace(FVector(400, 300, 0));

	requiredLocations.Emplace(FVector(100, 400, 0));
	requiredLocations.Emplace(FVector(200, 400, 0));
	requiredLocations.Emplace(FVector(300, 400, 0));
	requiredLocations.Emplace(FVector(400, 400, 0));

	return requiredLocations;
}

void UFenceGenerations::BuildFence(FVector loc, FRotator rot) {
	auto world = GetWorld();
	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, loc, rot] {
		auto actor = world->SpawnActor<AFence>(loc, rot);
		}, TStatId(), nullptr, ENamedThreads::GameThread);
}

void UFenceGenerations::BuildPlot(FVector loc) {
	auto world = GetWorld();
	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, loc] {
		auto actor = world->SpawnActor<ASoilPlot>(loc + FVector(50, 50, 0), FRotator::ZeroRotator);
		}, TStatId(), nullptr, ENamedThreads::GameThread);
}

#pragma optimize("", on)