// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/BaseAIBehaviour.h"
#include "AIMeleeRangeMove.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UAIMeleeRangeMove : public UBaseAIBehaviour
{
	GENERATED_BODY()

public:

	UAIMeleeRangeMove(const FObjectInitializer& ObjectInitializer);

	virtual void DoBehaviour() override;

protected:

	bool AttemptToRouteToPossibleLocation(TScriptInterface<ISvChar> character, FVector possibleLocation);
	
};
