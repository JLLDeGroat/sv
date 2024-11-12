// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseAIBehaviour.h"
#include "AiSuicideAttack.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UAiSuicideAttack : public UBaseAIBehaviour
{
	GENERATED_BODY()
	
public:

	UAiSuicideAttack(const FObjectInitializer& ObjectInitializer);

	virtual void DoBehaviour() override;

};
