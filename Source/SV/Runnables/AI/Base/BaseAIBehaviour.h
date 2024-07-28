// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseAIBehaviour.generated.h"

class ISvChar;

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

private:

	UPROPERTY() AActor* ThisEnemy;
	UPROPERTY() TArray<TScriptInterface<ISvChar>> AllCharacters;
	UPROPERTY() TArray<TScriptInterface<ISvChar>> BehaviourTargets;

	UPROPERTY() bool CompletedBehaviour;

};
