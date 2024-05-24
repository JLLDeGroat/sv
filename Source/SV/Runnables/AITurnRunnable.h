// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseRunnable.h"
#include "AITurnRunnable.generated.h"

class ISvChar;

/**
 * 
 */
UCLASS()
class SV_API UAITurnRunnable : public UBaseRunnable
{
	GENERATED_BODY()
public:

	virtual void ActivateThread() override;

private:

	TArray<TScriptInterface<ISvChar>> ClosestCharactersToThisEnemy(TScriptInterface<ISvChar> enemy, TArray<TScriptInterface<ISvChar>> characters);
	bool IsAlreadyAdjacentCharacter(FVector source, FVector characterLoc);
	bool AttemptToRouteToPossibleLocation(TScriptInterface<ISvChar> character, FVector possibleLocation);

	void TryMeleeAttack(TScriptInterface<ISvChar> meleeAttacker, TScriptInterface<ISvChar> character);
};
