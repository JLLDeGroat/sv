// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../Enums/EWorldEnums.h"
#include "Gameplay.generated.h"

class UCharacterManager;
class UObjectivesManager;

/**
 *
 */
UINTERFACE()
class SV_API UGameplay : public UInterface
{
	GENERATED_BODY()

};

class SV_API IGameplay {

	GENERATED_BODY()

public:

	virtual UCharacterManager* GetCharacterManager() = 0;
	virtual UObjectivesManager* GetObjectivesManager() = 0;

	virtual void EndTurn() = 0;
	virtual void BeginPlayerTurn() = 0;

	virtual bool AttemptToStartWinLossChecker() = 0;
	virtual void StartStatRunnable(AActor* statOwner, EStatisticType statType, float damage = 0.0f) = 0;
};