// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseAITurnManager.h"
#include "PreMoveChecker.generated.h"

class ISvChar;
class UBaseAIBehaviour;
/**
 *
 */
UCLASS()
class SV_API UPreMoveChecker : public UBaseAITurnManager
{
	GENERATED_BODY()

public:

	virtual void ActivateThread() override;

	TArray<TScriptInterface<ISvChar>> GetCurrentTarget();

private:
	void DetermineClosestCharactersToThisEnemy();

	UPROPERTY() TArray<TScriptInterface<ISvChar>> Targets;
	UPROPERTY() UBaseAIBehaviour* MeleeBehaviour;
};
