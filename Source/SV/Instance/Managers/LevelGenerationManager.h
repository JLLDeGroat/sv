// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LevelGenerationManager.generated.h"

/**
 * 
 */
UCLASS()
class SV_API ULevelGenerationManager : public UObject
{
	GENERATED_BODY()

public:

	void SetGridItems(TArray<FVector> arr);
	void SetSpawnLocations(TArray<FVector> arr);
	void SetEndLocations(TArray<FVector> arr);

	TArray<FVector> GetGridItems();
	TArray<FVector> GetSpawnLocations();
	TArray<FVector> GetEndLocations();


protected:

	UPROPERTY() TArray<FVector> GridItems;
	UPROPERTY() TArray<FVector> SpawnLocations;
	UPROPERTY() TArray<FVector> EndLocations;
	
};
