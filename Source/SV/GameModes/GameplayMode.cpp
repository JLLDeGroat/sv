// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayMode.h"
#include "../Hud/GameHud.h"
#include "../Player/GamePlayerController.h"
#include "../Player/PlayerPawn.h"
#include "Managers/TurnManager.h"
#include "Managers/CharacterManager.h"
#include "Managers/LevelSpawnerManager.h"
#include "Managers/WinLossManager.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Runnables/LevelGenerationRunnable.h"

AGameplayMode::AGameplayMode() {
	PlayerControllerClass = AGamePlayerController::StaticClass();
	DefaultPawnClass = APlayerPawn::StaticClass();
	HUDClass = AGameHud::StaticClass();

	CharacterManager = CreateDefaultSubobject<UCharacterManager>(TEXT("CharacterManager"));
	TurnManager = CreateDefaultSubobject<UTurnManager>(TEXT("TurnManager"));
	LevelSpawnerManager = CreateDefaultSubobject<ULevelSpawnerManager>(TEXT("SpawnerManager"));
	WinLossManager = CreateDefaultSubobject<UWinLossManager>(TEXT("WinLoss"));
}

void AGameplayMode::BeginPlay() {
	Super::BeginPlay();

	LevelGenThread = NewObject<ULevelGenerationRunnable>()
		->InsertVariables(ELevelGenType::TwoBuilding)
		->Initialise(GetWorld())
		->Begin();
}

UCharacterManager* AGameplayMode::GetCharacterManager() {
	return CharacterManager;
}
UWinLossManager* AGameplayMode::GetWinLossManager() {
	return WinLossManager;
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