// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseAITurnManager.h"
#include "PostMoveChecker.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UPostMoveChecker : public UBaseAITurnManager
{
	GENERATED_BODY()

public:
	virtual void ActivateThread() override;
	
};
