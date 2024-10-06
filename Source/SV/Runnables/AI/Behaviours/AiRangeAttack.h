// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/BaseAIBehaviour.h"
#include "AiRangeAttack.generated.h"


class ISvChar;

UCLASS()
class SV_API UAiRangeAttack : public UBaseAIBehaviour
{
	GENERATED_BODY()

public:


	UAiRangeAttack(const FObjectInitializer& ObjectInitializer);

	virtual void DoBehaviour() override;

private:

	FVector GetWhereToShootCharacter(TScriptInterface<ISvChar> character, FVector startLocation);
	
};
