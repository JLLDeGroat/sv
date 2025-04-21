// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../Enums/EWorldEnums.h"
#include "Gameplay.generated.h"

class UCharacterManager;
class UObjectivesManager;
class UOverwatchManager;
class UDirectorManager;
class UTurnManager;
class AFogManager;
/**
 *
 */
UINTERFACE()
class SV_API UGameplay : public UInterface
{
	GENERATED_BODY()
};

class SV_API IGameplay
{

	GENERATED_BODY()

public:
	virtual UCharacterManager *GetCharacterManager() = 0;
	virtual UObjectivesManager *GetObjectivesManager() = 0;
	virtual UOverwatchManager *GetOverwatchManager() = 0;
	virtual UDirectorManager *GetDirectorManager() = 0;
	virtual UTurnManager *GetTurnManager() = 0;
	virtual AFogManager *GetFogManager() = 0;
	virtual void SetFogManager(AFogManager *manager) = 0;

	virtual void EndTurn() = 0;
	virtual void BeginPlayerTurn() = 0;

	virtual bool AttemptToStartWinLossChecker() = 0;
	virtual void StartStatRunnable(AActor *statOwner, EStatisticType statType, float value = 0.0f) = 0;
	virtual void StartDirectorStatRunnable(AActor *statOwner, EDirectorStatType statType, float value = 0.0f) = 0;
};