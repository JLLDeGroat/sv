// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../Interfaces/Gameplay.h"
#include "../Runnables/Base/BaseRunnable.h"
#include "GameplayMode.generated.h"

class UCharacterManager;
class UTurnManager;
class ULevelSpawnerManager;
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

	ULevelSpawnerManager* GetLevelSpawnerManager();

	virtual void BeginDestroy() override;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UCharacterManager* CharacterManager;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UTurnManager* TurnManager;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) ULevelSpawnerManager* LevelSpawnerManager;

	UPROPERTY() UBaseRunnable* LevelGenThread;
	

};
