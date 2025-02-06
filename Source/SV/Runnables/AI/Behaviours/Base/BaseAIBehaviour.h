// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseAIBehaviour.generated.h"

class ISvChar;
class UAiMovementComponent;

/**
 *
 */
UCLASS()
class SV_API UBaseAIBehaviour : public UObject
{
	GENERATED_BODY()

public:

	UBaseAIBehaviour(const FObjectInitializer& ObjectInitializer);

	virtual void DoBehaviour();

	void SetEnemyAndCharacters(AActor* enemy, TArray<TScriptInterface<ISvChar>> allCharacters);
	void SetBehaviourTargets(TArray<TScriptInterface<ISvChar>> targets);
	void SetBehaviourTarget(TScriptInterface<ISvChar> targets);

	AActor* GetThisEnemy();
	TArray<TScriptInterface<ISvChar>> GetAllCharacters();
	TArray<TScriptInterface<ISvChar>> GetBehaviourTargets();

	bool GetCompletedBehaviour();
	bool GetCompletedBehaviourAndWaitIfNot(float seconds);
	UFUNCTION() void SetCompletedBehaviour();

	UAiMovementComponent* GetMovementComponent() { return MovementComponent; }

protected:

	void SpawnDebugGrid_SetIsStart(FVector location, float delay = .0001f);
	void SpawnDebugGrid_SetIsEnd(FVector location, float delay = .0001f);
	void SpawnDebugGrid_SetIsOffshoot(FVector location, float delay = .0001f);
	void SpawnDebugGrid_SetIsSpawn(FVector location, float delay = .0001f);
	void SpawnDebugGrid_SetIsObstacle(FVector location, float delay = .0001f);

	void SpawnDebugGrid_SetIsStart(TArray<FVector> locations, FVector offset = FVector(0, 0, -50), float delay = .0001f);
	void SpawnDebugGrid_SetIsEnd(TArray<FVector> locations, FVector offset = FVector(0, 0, -50), float delay = .0001f);
	void SpawnDebugGrid_SetIsOffshoot(TArray<FVector> locations, FVector offset = FVector(0, 0, -50), float delay = .0001f);
	void SpawnDebugGrid_SetIsSpawn(TArray<FVector> locations, FVector offset = FVector(0, 0, -50), float delay = .0001f);
	void SpawnDebugGrid_SetIsObstacle(TArray<FVector> locations, FVector offset = FVector(0, 0, -50), float delay = .0001f);

	TScriptInterface<ISvChar> GetClosestCharacter();
	FRandomStream GetRandomStream() { return _randomStream; }

	UPROPERTY() UAiMovementComponent* MovementComponent;

	bool CanMeleeAnyone();

	virtual void BeginDestroy() override;

	bool GetIsInFog();

private:

	UPROPERTY() AActor* ThisEnemy;
	UPROPERTY() TArray<TScriptInterface<ISvChar>> AllCharacters;
	UPROPERTY() TArray<TScriptInterface<ISvChar>> BehaviourTargets;

	UPROPERTY() bool CompletedBehaviour;

	UPROPERTY() FRandomStream _randomStream;

};
