// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayMode.h"
#include "../Hud/GameHud.h"
#include "../Player/GamePlayerController.h"
#include "../Player/PlayerPawn.h"
#include "Managers/TurnManager.h"
#include "Managers/CharacterManager.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

AGameplayMode::AGameplayMode() {
	PlayerControllerClass = AGamePlayerController::StaticClass();
	DefaultPawnClass = APlayerPawn::StaticClass();
	HUDClass = AGameHud::StaticClass();


	CharacterManager = CreateDefaultSubobject<UCharacterManager>(TEXT("CharacterManager"));
	TurnManager = CreateDefaultSubobject<UTurnManager>(TEXT("TurnManager"));
}

void AGameplayMode::BeginPlay() {
	Super::BeginPlay();
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