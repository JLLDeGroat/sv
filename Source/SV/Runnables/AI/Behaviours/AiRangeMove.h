// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/BaseAIBehaviour.h"
#include "AiRangeMove.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UAiRangeMove : public UBaseAIBehaviour
{
	GENERATED_BODY()
	
public:

	UAiRangeMove(const FObjectInitializer& ObjectInitializer);

	virtual void DoBehaviour() override;
};
