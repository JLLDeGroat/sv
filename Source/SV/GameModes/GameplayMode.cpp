// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayMode.h"
#include "../Hud/GameHud.h"
#include "../Player/GamePlayerController.h"
#include "../Player/PlayerPawn.h"
#include "Managers/TurnManager.h"
#include "Managers/CharacterManager.h"
#include "Managers/LevelSpawnerManager.h"
#include "Managers/ObjectivesManager.h"
#include "Managers/OverwatchManager.h"
#include "Managers/DirectorManager.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Runnables/LevelGenerationRunnable.h"
#include "../Runnables/Checkers/WinLossCheckerRunnable.h"
#include "../Runnables/Stats/StatUpdateRunnable.h"
#include "../Runnables/DirectorStats/DirectorStatRunnable.h"

AGameplayMode::AGameplayMode()
{
	PlayerControllerClass = AGamePlayerController::StaticClass();
	DefaultPawnClass = APlayerPawn::StaticClass();
	HUDClass = AGameHud::StaticClass();

	CharacterManager = CreateDefaultSubobject<UCharacterManager>(TEXT("CharacterManager"));
	TurnManager = CreateDefaultSubobject<UTurnManager>(TEXT("TurnManager"));
	LevelSpawnerManager = CreateDefaultSubobject<ULevelSpawnerManager>(TEXT("SpawnerManager"));
	ObjectivesManager = CreateDefaultSubobject<UObjectivesManager>(TEXT("ObjectiveManager"));
	OverwatchManager = CreateDefaultSubobject<UOverwatchManager>(TEXT("OverwatchManager"));
	DirectorManager = CreateDefaultSubobject<UDirectorManager>(TEXT("DirectorManager"));
}

void AGameplayMode::BeginPlay()
{
	Super::BeginPlay();
}

UObjectivesManager *AGameplayMode::GetObjectivesManager()
{
	return ObjectivesManager;
}

UCharacterManager *AGameplayMode::GetCharacterManager()
{
	return CharacterManager;
}
UTurnManager *AGameplayMode::GetTurnManager()
{
	return TurnManager;
}

void AGameplayMode::EndTurn()
{
	TurnManager->BeginAITurn();
}

void AGameplayMode::BeginPlayerTurn()
{
	UDebugMessages::LogDisplay(this, "Beginning Player Turn");
	TurnManager->BeginPlayerTurn();
}

ULevelSpawnerManager *AGameplayMode::GetLevelSpawnerManager()
{
	return LevelSpawnerManager;
}

void AGameplayMode::BeginDestroy()
{
	Super::BeginDestroy();
	TurnManager->KillRunnable();
}

void AGameplayMode::CleanupThreads()
{
	if (TurnManager)
		TurnManager->KillRunnable();
}

bool AGameplayMode::AttemptToStartWinLossChecker()
{
	if (!WinLossCheckerThread || WinLossCheckerThread->GetIsComplete())
	{
		WinLossCheckerThread = (UWinLossCheckerRunnable *)NewObject<UWinLossCheckerRunnable>()
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

void AGameplayMode::StartStatRunnable(AActor *statOwner, EStatisticType statType, float value)
{
	UDebugMessages::LogDisplay(this, "StartStatRunnable");
	auto newStatRunnable = (UStatUpdateRunnable *)NewObject<UStatUpdateRunnable>(this)
							   ->InsertVariables(statOwner, statType, value)
							   ->Initialise(GetWorld())
							   ->Begin();

	bool noChange = false;
	while (!noChange)
	{
		noChange = true;
		for (int i = 0; i < StatRunnables.Num(); i++)
		{
			if (!StatRunnables[i] || StatRunnables[i]->GetIsCompleteStatItem())
			{
				StatRunnables.RemoveAt(i);
				noChange = false;
				break;
			}
		}
	}

	StatRunnables.Emplace(newStatRunnable);
}

void AGameplayMode::StartDirectorStatRunnable(AActor *statOwner, EDirectorStatType statType, float value)
{
	UDebugMessages::LogDisplay(this, "StartDirectorStatRunnable");
	auto newStatRunnable = (UDirectorStatRunnable *)NewObject<UDirectorStatRunnable>(this)
							   ->InsertVariables(statOwner, statType, value)
							   ->Initialise(GetWorld())
							   ->Begin();

	bool noChange = false;
	while (!noChange)
	{
		noChange = true;
		for (int i = 0; i < DirectorRunnables.Num(); i++)
		{
			if (!DirectorRunnables[i] || DirectorRunnables[i]->GetIsCompleteStatItem())
			{
				DirectorRunnables.RemoveAt(i);
				noChange = false;
				break;
			}
		}
	}

	DirectorRunnables.Emplace(newStatRunnable);
}

UOverwatchManager *AGameplayMode::GetOverwatchManager()
{
	return OverwatchManager;
}

UDirectorManager *AGameplayMode::GetDirectorManager()
{
	return DirectorManager;
}
AFogManager *AGameplayMode::GetFogManager()
{
	return FogManager;
}
void AGameplayMode::SetFogManager(AFogManager *manager)
{
	FogManager = manager;
}