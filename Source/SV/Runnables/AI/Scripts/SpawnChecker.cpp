// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnChecker.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

#include "../../../Environment/Spawners/StaticEnemySpawner.h"
#include "../../../Characters/BaseCharacter.h"
#include "../../../Characters/Components/CharacterDetailsComponent.h"

USpawnChecker::USpawnChecker() {

}
#pragma optimize("", off)
void USpawnChecker::RunScript() {
	auto world = GetWorld();

	if (!world)
		return UDebugMessages::LogError(this, "could not get world");


	auto actors = world->GetCurrentLevel()->Actors;

	for (int i = 0; i < actors.Num(); i++) {
		if (actors[i]) {

			ABaseCharacter* character = Cast<ABaseCharacter>(actors[i]);
			if (character) {
				auto detailsComp = character->GetComponentByClass<UCharacterDetailsComponent>();

				if (!detailsComp)
					return UDebugMessages::LogError(this, "failed to get detailsComponent");

				if (detailsComp->GetIsDead()) {
					UDebugMessages::LogWarning(this, "warning, this enemy/player is dead so not counting");
					continue;
				}

				if (detailsComp->GetCharacterControl() == ECharacterControl::CC_Player)
					Players.Emplace(actors[i]);
				else if (detailsComp->GetCharacterControl() == ECharacterControl::CC_AI)
					Enemies.Emplace(actors[i]);
				else
					return UDebugMessages::LogError(this, "failed to get correct controller");

				continue;
			}

			AStaticEnemySpawner* enemySpawner = Cast<AStaticEnemySpawner>(actors[i]);
			if (enemySpawner) Spawners.Emplace(enemySpawner);
		}
	}

	DetermineValidSpawners();

	if (ValidSpawners.Num() > 0) {
		auto spawnWeight = 50;

		if (Enemies.Num() < 3) {
			spawnWeight += 100;
		}

		if (Enemies.Num())
			spawnWeight += 200;

		FRandomStream stream = FRandomStream(123);

		auto toSpawn = 0;
		auto random = stream.RandRange(0, 300);
		while (spawnWeight > random) {
			toSpawn += 1;
			spawnWeight -= stream.RandRange(30, 60);
		}

		for (int i = 0; i < ValidSpawners.Num(); i++) {

			auto thisSpawner = ValidSpawners[i];
			FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([thisSpawner]
				{
					thisSpawner->StartSpawn();
				},
				TStatId(), nullptr, ENamedThreads::GameThread);

			toSpawn -= 1;
			if (toSpawn == 0) {
				break;
			}
		}
	}
}

void USpawnChecker::DetermineValidSpawners() {
	for (int i = 0; i < Spawners.Num(); i++) {
		bool bIsValid = true;
		auto spawnerLoc = Spawners[i]->GetActorLocation();
		for (int x = 0; x < Enemies.Num(); x++) {
			auto dist = FVector::Dist(Enemies[x]->GetActorLocation(), spawnerLoc);
			if (dist < 150) bIsValid = false;
		}

		for (int x = 0; x < Players.Num(); x++) {
			auto dist = FVector::Dist(Players[x]->GetActorLocation(), spawnerLoc);
			if (dist < 150) bIsValid = false;
		}

		if (bIsValid)
			ValidSpawners.Emplace(Spawners[i]);
	}
}
#pragma optimize("", on)