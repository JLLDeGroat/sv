// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "../../Interfaces/HitComponent.h"
#include "HitCapsuleComponent.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UHitCapsuleComponent : public UCapsuleComponent, public IHitComponent
{
	GENERATED_BODY()

public:

	UHitCapsuleComponent(const FObjectInitializer& ObjectInitializer);

	void SetHitDimageMultiplier(float value);

	virtual FVector GetWorldLocation() override;
	virtual float GetHitDamageMultiplier() override;
private:

	UPROPERTY() float DamageMultiplier;
};
