// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../Interfaces/Gameplay.h"
#include "../Runnables/Base/BaseRunnable.h"
#include "../Enums/EWorldEnums.h"
#include "GameplayMode.generated.h"

class UCharacterManager;
class UTurnManager;
class ULevelSpawnerManager;
class UObjectivesManager;
class UWinLossCheckerRunnable;
class UStatUpdateRunnable;
class UOverwatchManager;
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
	virtual UObjectivesManager* GetObjectivesManager() override;
	virtual UOverwatchManager* GetOverwatchManager() override;
	virtual void EndTurn() override;
	virtual void BeginPlayerTurn() override;

	ULevelSpawnerManager* GetLevelSpawnerManager();

	void CleanupThreads();
	virtual void BeginDestroy() override;

	virtual bool AttemptToStartWinLossChecker() override;
	virtual void StartStatRunnable(AActor* statOwner, EStatisticType statType, float damage = 0.0f);

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UCharacterManager* CharacterManager;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UTurnManager* TurnManager;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) ULevelSpawnerManager* LevelSpawnerManager;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UObjectivesManager* ObjectivesManager;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UOverwatchManager* OverwatchManager;
	UPROPERTY() UBaseRunnable* LevelGenThread;
	UPROPERTY() UWinLossCheckerRunnable* WinLossCheckerThread;
	UPROPERTY() TArray<UStatUpdateRunnable*> StatRunnables;
};
