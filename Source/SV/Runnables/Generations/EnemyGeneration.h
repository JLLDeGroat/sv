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
	virtual UBaseGenerations *Generate() override;

	UEnemyGeneration *SetTotalWalkingZombies(int amount);

	UEnemyGeneration *SetPodAmountAndTier(int amount, int maxTier);

private:
	TArray<FVector> GeneratePodSpawnLocations(FVector startLocation);
	bool IsValidPodLocations(TArray<FVector> podLocs);

	bool IsTooCloseToSpawnZone(FVector location, float allowedDistance);
	UPROPERTY()
	int PodAmount = 4;
	UPROPERTY()
	int PodTierAmount = 1;
	// default values
	UPROPERTY()
	int TotalWalkingZombies = 5;
	UPROPERTY()
	TArray<FVector> AllEnemyLocations;

	bool IsWithinList(FVector loc, TArray<FVector> location);
	bool IsTooCloseToOtherSpawns(FVector loc, TArray<FVector> location, float distance = 350);

	UPROPERTY()
	TArray<FVector> PodLocations;
	UPROPERTY()
	TArray<FVector> EnemySpawnLocations;
};
