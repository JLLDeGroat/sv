// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SpawnChecker.generated.h"

class ABaseCharacter;
class AStaticEnemySpawner;

/**
 *
 */
UCLASS()
class SV_API USpawnChecker : public UObject
{
	GENERATED_BODY()

public:
	USpawnChecker();

	void RunScript();


private:

	void DetermineValidSpawners();

	UPROPERTY() TArray<ABaseCharacter*> Players;
	UPROPERTY() TArray<ABaseCharacter*> Enemies;

	UPROPERTY() TArray<AStaticEnemySpawner*> Spawners;
	UPROPERTY() TArray<AStaticEnemySpawner*> ValidSpawners;

};
