// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "../../Interfaces/HitComponent.h"
#include "HitBoxComponent.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UHitBoxComponent : public UBoxComponent, public IHitComponent
{
	GENERATED_BODY()
	
public:

	UHitBoxComponent(const FObjectInitializer& ObjectInitializer);

	void SetHitDimageMultiplier(float value);
	void SetThickness(int thickness);

	virtual FVector GetWorldLocation() override;
	virtual float GetHitDamageMultiplier() override;
	virtual int GetThickness() override;

private:

	UPROPERTY() float DamageMultiplier;
	UPROPERTY() int Thickness;
};
