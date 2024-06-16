// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuGameMode.h"
#include "../Player/MenuPlayerController.h"
#include "../Hud/MenuHud.h"
#include "../Player/MenuPawn.h"

AMenuGameMode::AMenuGameMode() {
	PlayerControllerClass = AMenuPlayerController::StaticClass();
	HUDClass = AMenuHud::StaticClass();
	DefaultPawnClass = AMenuPawn::StaticClass();
}