// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitComponent.generated.h"

/**
 *
 */
UINTERFACE(BlueprintType)
class SV_API UHitComponent : public UInterface
{
	GENERATED_BODY()

};

class SV_API IHitComponent {

	GENERATED_BODY()

public:

	virtual FVector GetWorldLocation() = 0;

	virtual float GetHitDamageMultiplier() = 0;

};

