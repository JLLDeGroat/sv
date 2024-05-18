// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayMode.h"
#include "../Hud/GameHud.h"
#include "../Player/GamePlayerController.h"
#include "../Player/PlayerPawn.h"
#include "Managers/CharacterManager.h"

AGameplayMode::AGameplayMode() {
	PlayerControllerClass = AGamePlayerController::StaticClass();
	DefaultPawnClass = APlayerPawn::StaticClass();
	HUDClass = AGameHud::StaticClass();


	CharacterManager = NewObject<UCharacterManager>();
}

void AGameplayMode::BeginPlay() {
	Super::BeginPlay();
	CharacterManager->AssignDelegates();
}

UCharacterManager* AGameplayMode::GetCharacterManager() {
	return CharacterManager;
}