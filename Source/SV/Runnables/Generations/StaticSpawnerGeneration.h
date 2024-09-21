// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseGenerations.h"
#include "StaticSpawnerGeneration.generated.h"

/**
 *
 */
UCLASS()
class SV_API UStaticSpawnerGeneration : public UBaseGenerations
{
	GENERATED_BODY()

public:

	UStaticSpawnerGeneration();
	virtual UBaseGenerations* Generate() override;
	UStaticSpawnerGeneration* SetAmountToSpawn(int amount);
protected:

	UPROPERTY() int AmountToSpawn;
	UPROPERTY() TArray<FVector> LocationsToSpawn;
};
