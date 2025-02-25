// Fill out your copyright notice in the Description page of Project Settings.


#include "FenceGenerations.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Environment/Constructions/Fence.h"
#include "../../Environment/Natural/SoilPlot.h"
#include "../../World/WorldGridItemActor.h"


UBaseGenerations* UFenceGenerations::Generate() {
	TemplatedRequiredLocations = GenerateRequiredLocations();

	for (int i = 0; i < UsableLocations.Num(); i++) {
		if (CanFitInLocation(UsableLocations[i])) {
			if (ShouldGenerate()) {
				DecrementChance();

				auto thisRequiredLocation = CreateRequiredLocations(UsableLocations[i]);
				RequiredLocations = CombineList(thisRequiredLocation, RequiredLocations);
				TotalUsedLocations = CombineList(TotalUsedLocations, RequiredLocations);

				for (int x = 0; x < thisRequiredLocation.Num(); x++) {
					if (IsRequiredSpotBottomRight(thisRequiredLocation[x])) {
						BuildFence(thisRequiredLocation[x] + FVector(50, 50, 0), FRotator(0, 180, 0));
						BuildFence(thisRequiredLocation[x] + FVector(50, 150, 0), FRotator(0, 90, 0));
					}
					else if (IsRequiredSpotBottomLeft(thisRequiredLocation[x])) {
						BuildFence(thisRequiredLocation[x] + FVector(50, 50, 0), FRotator(0, 180, 0));
						BuildFence(thisRequiredLocation[x] + FVector(50, 50, 0), FRotator(0, 90, 0));
					}
					else if (IsRequiredSpotTopRight(thisRequiredLocation[x])) {
						BuildFence(thisRequiredLocation[x] + FVector(50, 150, 0), FRotator(0, 90, 0));
						BuildFence(thisRequiredLocation[x] + FVector(150, 150, 0));
					}
					else if (IsRequiredSpotTopLeft(thisRequiredLocation[x])) {
						BuildFence(thisRequiredLocation[x] + FVector(150, 50, 0), FRotator(0, 180, 0));
						BuildFence(thisRequiredLocation[x] + FVector(50, 50, 0), FRotator(0, 90, 0));
					}
					else {

						if (IsRequiredSpotLeftWall(thisRequiredLocation[x])) {
							BuildFence(thisRequiredLocation[x] + FVector(50, 50, 0), FRotator(0, 90, 0));
						}
						else if (IsRequiredSpotRightWall(thisRequiredLocation[x])) {
							BuildFence(thisRequiredLocation[x] + FVector(50, 150, 0), FRotator(0, 90, 0));
						}
						else if (IsRequiredSpotBottomWall(thisRequiredLocation[x])) {
							BuildFence(thisRequiredLocation[x] + FVector(50, 50, 0), FRotator(0, 180, 0));
						}
						else if (IsRequiredSpotTopWall(thisRequiredLocation[x])) {
							BuildFence(thisRequiredLocation[x] + FVector(150, 50, 0), FRotator(0, 180, 0));
						}
						else {
							auto num = RandomStream.RandRange(1, 1001);
							if (num > 500)
								BuildPlot(thisRequiredLocation[x]);
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
