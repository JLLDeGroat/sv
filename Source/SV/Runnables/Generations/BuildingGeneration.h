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

	virtual UBaseGenerations* Generate() override;
	

private:

	TArray<FVector> GetBuildingRequiredLocations();
	void BuildWall(FVector loc, FRotator rot = FRotator::ZeroRotator);
	void BuildDoor(FVector loc, FRotator rot = FRotator::ZeroRotator);

};
