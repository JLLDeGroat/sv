// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "../Enums/EWorldEnums.h"
#include "../Enums/EEquipmentEnums.h"
#include "SpawnUtilities.generated.h"
/**
 *
 */

class ABaseCharacter;
class AStaticEnemySpawner;
UCLASS()
class SV_API USpawnUtilities : public UObject
{
	GENERATED_BODY()
public:
static AActor *SpawnEnemyOfTypeAtLocation(UWorld *world, TSubclassOf<ABaseCharacter> baseCharacter, FVector loc, AStaticEnemySpawner* enemySpawner = nullptr, bool spawnFromFloor = false);


private:
};
