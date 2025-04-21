// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelGenerationRunnable.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Generations/LevelIndex/GenericLevel.h"
#include "Generations/LevelIndex/TwoBuildingsLevel.h"
#include "../GameModes/Managers/DirectorManager.h"
#include "../Utilities/SvUtilities.h"
#include "../Interfaces/Gameplay.h"
#include "../Environment/Fog/FogManager.h"

ULevelGenerationRunnable *ULevelGenerationRunnable::InsertVariables(ELevelGenType levelGen)
{
	GenType = levelGen;
	return this;
}

void ULevelGenerationRunnable::KillThread()
{
	Super::KillThread();
	if (LevelIndex)
	{
		LevelIndex->KillObject();
	}
}

void ULevelGenerationRunnable::ActivateThread()
{
	LevelIndex = nullptr;

	switch (GenType)
	{
	case ELevelGenType::TwoBuilding:
		LevelIndex = NewObject<UTwoBuildingsLevel>();
		break;
	case ELevelGenType::Generic:
		LevelIndex = NewObject<UGenericLevel>();
		break;

	default:
		UDebugMessages::LogError(this, "no gen class for this level index");
	}

	if (LevelIndex)
	{
		LevelIndex
			->Setup(GetWorld(), RandomStream)
			->GenerateLevel();

		auto allLeftAllowedLocations = LevelIndex->GetObstacleAllowedLocations();
		auto directorManager = USvUtilities::GetGameModeDirectorManager(GetWorld());
		directorManager->SetAvailableFloorSpawns(allLeftAllowedLocations);

		auto actors = GetWorld()->GetCurrentLevel()->Actors;
		for (AActor *actor : actors)
		{
			if (actor && Cast<AFogManager>(actor))
				USvUtilities::GetGameMode(GetWorld())->SetFogManager(Cast<AFogManager>(actor));
		}
	}
}
