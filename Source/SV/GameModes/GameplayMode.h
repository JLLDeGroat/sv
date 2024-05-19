// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../Interfaces/Gameplay.h"
#include "GameplayMode.generated.h"

class UCharacterManager;
class UTurnManager;
/**
 * 
 */
UCLASS()
class SV_API AGameplayMode : public AGameModeBase, public IGameplay
{
	GENERATED_BODY()

public:

	AGameplayMode();

	virtual void BeginPlay() override;

	//inherited from IGameplay
	virtual UCharacterManager* GetCharacterManager() override;
	virtual void EndTurn() override;
	virtual void BeginPlayerTurn() override;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UCharacterManager* CharacterManager;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UTurnManager* TurnManager;

};
