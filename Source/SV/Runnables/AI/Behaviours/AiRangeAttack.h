// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseAIBehaviour.h"
#include "Data/FAIRangeAttackData.h"
#include "AiRangeAttack.generated.h"

class IHitComponent;
class ISvChar;

UCLASS()
class SV_API UAiRangeAttack : public UBaseAIBehaviour
{
	GENERATED_BODY()

public:
	UAiRangeAttack(const FObjectInitializer &ObjectInitializer);

	virtual void DoBehaviour() override;

private:
	FWhereToShoot GetWhereToShootCharacter(TScriptInterface<ISvChar> character, FVector startLocation);

	FWhereToShoot DeterminePossibleShootLocation(FVector startLocation, TScriptInterface<IHitComponent> hitComponent);
};
