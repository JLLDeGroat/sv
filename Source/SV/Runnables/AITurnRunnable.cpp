// Fill out your copyright notice in the Description page of Project Settings.


#include "AITurnRunnable.h"
#include "../GameModes/GameplayMode.h"
#include "../Interfaces/Gameplay.h"

void UAITurnRunnable::ActivateThread() {
	UDebugMessages::LogDisplay(this, "Starting AI turns");
	TScriptInterface<IGameplay> gamePlay = GetWorld()->GetAuthGameMode<AGameplayMode>();
	
	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([gamePlay]
		{
			gamePlay->BeginPlayerTurn();
		},
		TStatId(), nullptr, ENamedThreads::GameThread);
}