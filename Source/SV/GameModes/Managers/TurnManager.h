// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TurnManager.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UTurnManager : public UObject
{
	GENERATED_BODY()

public:
	UTurnManager(const FObjectInitializer& ObjectInitializer);
	void BeginAITurn();
	void BeginPlayerTurn();
};