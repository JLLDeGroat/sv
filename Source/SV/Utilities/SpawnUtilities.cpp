// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnUtilities.h"

#include "../Characters/BaseCharacter.h"
#include "../Characters/Components/SpawnInComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Environment/Spawners/StaticEnemySpawner.h"

AActor *USpawnUtilities::SpawnEnemyOfTypeAtLocation(UWorld *world, TSubclassOf<ABaseCharacter> baseCharacter, FVector loc, AStaticEnemySpawner *enemySpawner, bool spawnFromFloor)
{
	auto newGrunt = world->SpawnActor<ABaseCharacter>(baseCharacter, loc, FRotator(0));
	if (newGrunt)
	{
		auto spawnInComponent = newGrunt->GetComponentByClass<USpawnInComponent>();
		if (!spawnInComponent)
		{
			UDebugMessages::LogError(world, "enemy does not have spawn in component... deleting");
			newGrunt->Destroy();
			return nullptr;
		}

		if (!enemySpawner)
		{
			enemySpawner = world->SpawnActor<AStaticEnemySpawner>(loc, FRotator(0));
		}

		if (spawnFromFloor)
		{
			enemySpawner->StartGroundSpawnEffects();
			spawnInComponent->StartSpawnInFromGround();
		}
		else
		{
			enemySpawner->StartWallSpawnEffects();
			spawnInComponent->StartSpawnInFromWall();
		}
	}
	return nullptr;
}