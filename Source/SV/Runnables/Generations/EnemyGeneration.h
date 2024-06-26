// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseGenerations.h"
#include "EnemyGeneration.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UEnemyGeneration : public UBaseGenerations
{
	GENERATED_BODY()
	
public:

	virtual UBaseGenerations* Generate() override;

	UEnemyGeneration* SetTotalWalkingZombies(int amount);

private:

	bool IsTooCloseToSpawnZone(FVector location, float allowedDistance);

	// default values
	UPROPERTY() int TotalWalkingZombies = 5;
	UPROPERTY() TArray<FVector> AllEnemyLocations;

	bool IsWithinList(FVector loc, TArray<FVector> location);
	bool IsTooCloseToOtherSpawns(FVector loc, TArray<FVector> location, float distance = 350);
};
