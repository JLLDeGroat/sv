// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseGenerations.h"
#include "BuildingGeneration.generated.h"

/**
 *
 */
UCLASS()
class SV_API UBuildingGeneration : public UBaseGenerations
{
	GENERATED_BODY()

public:

	UBuildingGeneration();

	virtual UBaseGenerations* Generate() override;

	UBuildingGeneration* SetXandYWidth(int x, int y);
	UBuildingGeneration* SetAmountToGenerate(int amount);

	TArray<FVector> GetDoorLocations();

private:

	TArray<FVector> GetBuildingRequiredLocations();
	void BuildWall(FVector loc, FRotator rot = FRotator::ZeroRotator);
	void BuildDoor(FVector loc, FVector offset, FRotator rot = FRotator::ZeroRotator);
	void BuildWindow(FVector loc, FRotator rot = FRotator::ZeroRotator);
	void BuildFloor(FVector loc);

	bool ShouldBuildWindow();

	UPROPERTY() TArray<FVector> DoorLocations;
	UPROPERTY() TArray<FVector> ThisBuildingsDoors;

	UPROPERTY() int XWidth = 0;
	UPROPERTY() int YWidth = 0;
	UPROPERTY() int AmountToGenerate;
	UPROPERTY() int AmountGenerated;

	void GenerateBuildingPoint(FVector loc, bool& bgeneratedLeftWindows, bool& bgeneratedRighttWindows, bool& bgeneratedUpWindows, bool& bgeneratedDownWindows, bool& generatedDoor);

};
