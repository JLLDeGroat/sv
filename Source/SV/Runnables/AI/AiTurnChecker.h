// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseAITurnManager.h"
#include "AiTurnChecker.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UAiTurnChecker : public UBaseAITurnManager
{
	GENERATED_BODY()

public:

	UAiTurnChecker(const FObjectInitializer& ObjectInitializer);


	bool GetCanProceedToNextAiUnit() const;
	bool GetCanProceedAndWaitIfNot(int seconds) const;
	void SetCanProceedToNextAiUnit(bool value);

protected:

	UFUNCTION() void OnCharacterFinishedTurn();

private:

	UPROPERTY() bool bCanProceedToNextAiUnit;
	
};
