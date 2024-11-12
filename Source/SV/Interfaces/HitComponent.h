// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitComponent.generated.h"

class UModularSkeletonComponent;
class AEquipment;

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
	virtual int GetThickness() = 0;
	virtual int GetSpatterBackDistance() = 0;
	virtual void SetModularComponent(UModularSkeletonComponent* component) = 0;
	virtual UModularSkeletonComponent* GetModularComponent() = 0;
	virtual void DamageModularComponent(int amount, FVector direction) = 0;

	virtual void AddEquipmentAsModularChild(AEquipment* equipment) = 0;
};

