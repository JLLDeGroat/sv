// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayMode.h"
#include "../Hud/GameHud.h"
#include "../Player/GamePlayerController.h"
#include "../Player/PlayerPawn.h"
#include "Managers/TurnManager.h"
#include "Managers/CharacterManager.h"
#include "Managers/LevelSpawnerManager.h"
#include "Managers/ObjectivesManager.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Runnables/LevelGenerationRunnable.h"
#include "../Runnables/Checkers/WinLossCheckerRunnable.h"
#include "../Runnables/Stats/StatUpdateRunnable.h"

AGameplayMode::AGameplayMode() {
	PlayerControllerClass = AGamePlayerController::StaticClass();
	DefaultPawnClass = APlayerPawn::StaticClass();
	HUDClass = AGameHud::StaticClass();

	CharacterManager = CreateDefaultSubobject<UCharacterManager>(TEXT("CharacterManager"));
	TurnManager = CreateDefaultSubobject<UTurnManager>(TEXT("TurnManager"));
	LevelSpawnerManager = CreateDefaultSubobject<ULevelSpawnerManager>(TEXT("SpawnerManager"));
	ObjectivesManager = CreateDefaultSubobject<UObjectivesManager>(TEXT("ObjectiveManager"));
}

void AGameplayMode::BeginPlay() {
	Super::BeginPlay();

	LevelGenThread = NewObject<ULevelGenerationRunnable>()
		->InsertVariables(ELevelGenType::TwoBuilding)
		->Initialise(GetWorld())
		->Begin();
}

UObjectivesManager* AGameplayMode::GetObjectivesManager() {
	return ObjectivesManager;
}

UCharacterManager* AGameplayMode::GetCharacterManager() {
	return CharacterManager;
}

void AGameplayMode::EndTurn() {
	TurnManager->BeginAITurn();
}

void AGameplayMode::BeginPlayerTurn() {
	UDebugMessages::LogDisplay(this, "Beginning Player Turn");
	TurnManager->BeginPlayerTurn();
}

ULevelSpawnerManager* AGameplayMode::GetLevelSpawnerManager() {
	return LevelSpawnerManager;
}

void AGameplayMode::BeginDestroy() {
	TurnManager->KillRunnable();
	Super::BeginDestroy();
}

bool AGameplayMode::AttemptToStartWinLossChecker() {
	if (!WinLossCheckerThread || WinLossCheckerThread->GetIsComplete()) {
		WinLossCheckerThread = (UWinLossCheckerRunnable*)NewObject<UWinLossCheckerRunnable>()
			->Initialise(GetWorld())
			->Begin();
		return true;
	}
	else
	{
		UDebugMessages::LogError(this, "could not attempt to start win loss checker");
		return false;
	}
}

void AGameplayMode::StartStatRunnable(AActor* statOwner, EStatisticType statType, float damage) {
	UDebugMessages::LogDisplay(this, "StartStatRunnable");
	auto newStatRunnable = (UStatUpdateRunnable*)NewObject<UStatUpdateRunnable>(this)
		->InsertVariables(statOwner, statType, damage)
		->Initialise(GetWorld())
		->Begin();

	bool noChange = false;
	while (!noChange) {
		noChange = true;
		for (int i = 0; i < StatRunnables.Num(); i++) {
			if (!StatRunnables[i] || StatRunnables[i]->GetIsCompleteStatItem()) {
				StatRunnables.RemoveAt(i);
				noChange = false;
				break;
			}
		}
	}

	StatRunnables.Emplace(newStatRunnable);
}