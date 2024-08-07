// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/BaseRunnable.h"
#include "WinLossCheckerRunnable.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UWinLossCheckerRunnable : public UBaseRunnable
{
	GENERATED_BODY()

public:
	virtual void ActivateThread() override;

private:

	bool GetHasAllCharactersLeftTheField();
	bool GetHasExtractedAnyCharacters();
	
	bool GetHasExterminationComplete();


	void SetHasCompletedThisMission();

};
