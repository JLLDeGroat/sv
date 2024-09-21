// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingGeneration.h"
#include "../../World/WorldGridItemActor.h"
#include "../../Environment/Constructions/Wall.h"
#include "../../Environment/Constructions/Door.h"
#include "../../Environment/Constructions/WallWindow.h"
#include "../../Environment/Constructions/CementFloor.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "BuildingInteriorGeneration.h"


UBuildingGeneration::UBuildingGeneration() {
	AmountToGenerate = 3;
	AmountGenerated = 0;
	XWidth = 0;
	YWidth = 0;
}
UBuildingGeneration* UBuildingGeneration::SetXandYWidth(int x, int y) {
	XWidth = x;
	YWidth = y;
	return this;
}
UBuildingGeneration* UBuildingGeneration::SetAmountToGenerate(int amount) {
	AmountToGenerate = amount;
	return this;
}
TArray<FVector> UBuildingGeneration::GetDoorLocations() {
	return DoorLocations;
}


UBaseGenerations* UBuildingGeneration::Generate() {
	UDebugMessages::LogDisplay(this, "Generating Building: allowed spots " + FString::SanitizeFloat(UsableLocations.Num()));
	TemplatedRequiredLocations = GetBuildingRequiredLocations();
	for (int i = 0; i < UsableLocations.Num(); i++) {
		if (CanFitInLocation(UsableLocations[i])) {
			if (ShouldGenerate()) {
				AmountGenerated++;
				DecrementChance();

				bool generatedDoor = false;

				auto thisRequiredLocationList = CreateRequiredLocations(UsableLocations[i]);
				RequiredLocations = CombineList(RequiredLocations, thisRequiredLocationList);
				RequiredBufferLocations = CombineList(RequiredBufferLocations, GenerateBufferAroundLocations(RequiredLocations, 2));
				TotalUsedLocations = CombineList(CombineList(TotalUsedLocations, RequiredLocations), RequiredBufferLocations);
				//AddToUsedSpots(requiredLocs);

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

				for (int x = 0; x < thisRequiredLocationList.Num(); x++) {
					GenerateBuildingPoint(thisRequiredLocationList[x], bgeneratedLeftWindows, bgeneratedRighttWindows, bgeneratedUpWindows, bgeneratedDownWindows, generatedDoor);
				}

				auto buildingInteriorGenerator = NewObject<UBuildingInteriorGeneration>(this)
					->SetDoorLocations(ThisBuildingsDoors)
					->SetInternalRouteAmount(1 + thisRequiredLocationList.Num() / 25)
					->SetupGeneration(GetWorld(), RandomStream, thisRequiredLocationList)
					->Generate();

				ThisBuildingsDoors.Empty();
			}
			else {
				//reset requiredlocations for new house size
				TemplatedRequiredLocations = GetBuildingRequiredLocations();
				IncrementChance();
			}

			if (AmountGenerated >= AmountToGenerate)
				return this;
		}
	}
	return this;
}

TArray<FVector> UBuildingGeneration::GetBuildingRequiredLocations() {

	TArray<FVector> result;

	auto random = 600; //RandomStream.RandRange(1, 1001);

	auto randomLength = XWidth == 0 ? RandomStream.RandRange(3, 8) : XWidth;
	auto randomWidth = YWidth == 0 ? RandomStream.RandRange(3, 4) : YWidth;

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

void UBuildingGeneration::BuildFloor(FVector loc) {
	auto world = GetWorld();

	auto rotMultiplier = RandomStream.RandRange(0, 4);

	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, loc, rotMultiplier] {
		auto actor = world->SpawnActor<ACementFloor>(loc, FRotator(0, 90 * rotMultiplier, 0));
		}, TStatId(), nullptr, ENamedThreads::GameThread);
}

void UBuildingGeneration::BuildWall(FVector loc, FRotator rot) {
	auto world = GetWorld();
	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, loc, rot] {
		auto actor = world->SpawnActor<AWall>(loc, rot);
		}, TStatId(), nullptr, ENamedThreads::GameThread);
}

void UBuildingGeneration::BuildDoor(FVector loc, FVector offset, FRotator rot) {
	DoorLocations.Emplace(loc);
	ThisBuildingsDoors.Emplace(loc);
	auto finalLoc = loc + offset;
	auto world = GetWorld();
	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, finalLoc, rot] {
		auto actor = world->SpawnActor<ADoor>(finalLoc, rot);
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

void UBuildingGeneration::GenerateBuildingPoint(FVector loc, bool& bgeneratedLeftWindows, bool& bgeneratedRighttWindows,
	bool& bgeneratedUpWindows, bool& bgeneratedDownWindows, bool& generatedDoor) {
	BuildFloor(loc + FVector(0, 0, 0));
	if (IsRequiredSpotBottomRight(loc)) {
		BuildWall(loc + FVector(-50, -50, 0), FRotator(0, 180, 0));
		BuildWall(loc + FVector(-50, 50, 0), FRotator(0, 90, 0));
	}
	else if (IsRequiredSpotBottomLeft(loc)) {
		BuildWall(loc + FVector(-50, -50, 0), FRotator(0, 180, 0));
		BuildWall(loc + FVector(-50, -50, 0), FRotator(0, 90, 0));
	}
	else if (IsRequiredSpotTopRight(loc)) {
		BuildWall(loc + FVector(-50, 50, 0), FRotator(0, 90, 0));
		BuildWall(loc + FVector(50, 50, 0));
	}
	else if (IsRequiredSpotTopLeft(loc)) {
		BuildWall(loc + FVector(50, -50, 0), FRotator(0, 180, 0));
		BuildWall(loc + FVector(-50, -50, 0), FRotator(0, 90, 0));
	}
	else {

		if (IsRequiredSpotLeftWall(loc)) {
			if (!generatedDoor) {
				BuildDoor(loc, FVector(-50, -50, 0), FRotator(0, 90, 0));
				generatedDoor = true;
			}
			else {
				if (!bgeneratedLeftWindows) {
					if (!ShouldBuildWindow()) {
						BuildWall(loc + FVector(-50, -50, 0), FRotator(0, 90, 0));
						bgeneratedLeftWindows = true;
					}
					else BuildWindow(loc + FVector(-50, -50, 0), FRotator(0, 90, 0));
				}
				else BuildWall(loc + FVector(-50, -50, 0), FRotator(0, 90, 0));

			}
		}
		else if (IsRequiredSpotRightWall(loc)) {
			if (!generatedDoor) {
				BuildDoor(loc, FVector(-50, 50, 0), FRotator(0, 90, 0));
				generatedDoor = true;
			}
			else {
				if (!bgeneratedRighttWindows) {
					if (!ShouldBuildWindow()) {
						BuildWall(loc + FVector(-50, 50, 0), FRotator(0, 90, 0));
						bgeneratedRighttWindows = true;
					}
					else BuildWindow(loc + FVector(-50, 50, 0), FRotator(0, 90, 0));
				}
				else BuildWall(loc + FVector(-50, 50, 0), FRotator(0, 90, 0));

			}
		}
		else if (IsRequiredSpotBottomWall(loc)) {
			if (!generatedDoor) {
				BuildDoor(loc, FVector(-50, -50, 0), FRotator(0, 180, 0));
				generatedDoor = true;
			}
			else {
				if (!bgeneratedDownWindows) {
					if (!ShouldBuildWindow()) {
						BuildWall(loc + FVector(-50, -50, 0), FRotator(0, 180, 0));
						bgeneratedDownWindows = true;
					}
					else BuildWindow(loc + FVector(-50, -50, 0), FRotator(0, 180, 0));
				}
				else BuildWall(loc + FVector(-50, -50, 0), FRotator(0, 180, 0));

			}
		}
		else if (IsRequiredSpotTopWall(loc)) {
			if (!generatedDoor) {
				BuildDoor(loc, FVector(50, -50, 0), FRotator(0, 180, 0));
				generatedDoor = true;
			}
			else {
				if (!bgeneratedUpWindows) {
					if (!ShouldBuildWindow()) {
						BuildWall(loc + FVector(50, -50, 0), FRotator(0, 180, 0));
						bgeneratedUpWindows = true;
					}
					else BuildWindow(loc + FVector(50, -50, 0), FRotator(0, 180, 0));
				}
				else BuildWall(loc + FVector(50, -50, 0), FRotator(0, 180, 0));
			}
		}
	}
}

