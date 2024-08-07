// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Gameplay.generated.h"

class UCharacterManager;
class UWinLossManager;

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
	virtual UWinLossManager* GetWinLossManager() = 0;

	virtual void EndTurn() = 0;
	virtual void BeginPlayerTurn() = 0;
};