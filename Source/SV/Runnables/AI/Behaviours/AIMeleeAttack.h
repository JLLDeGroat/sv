// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/BaseAIBehaviour.h"
#include "AIMeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UAIMeleeAttack : public UBaseAIBehaviour
{
	GENERATED_BODY()
	
public:

	UAIMeleeAttack(const FObjectInitializer& ObjectInitializer);

	virtual void DoBehaviour() override;

private:

	bool CanMeleeAnyone();

};
