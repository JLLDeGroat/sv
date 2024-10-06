// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseAITurnManager.h"
#include "AiTurnMoveChecker.generated.h"

class UBaseAIBehaviour;
/**
 * 
 */
UCLASS()
class SV_API UAiTurnMoveChecker : public UBaseAITurnManager
{
	GENERATED_BODY()
	
public:

	virtual void ActivateThread() override;

protected:

	UPROPERTY() UBaseAIBehaviour* MovementBehaviour;

};
