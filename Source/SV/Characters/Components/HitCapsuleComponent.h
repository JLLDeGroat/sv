// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "../../Interfaces/HitComponent.h"
#include "HitCapsuleComponent.generated.h"

class AEquipment;
class UModularSkeletonComponent;

UCLASS()
class SV_API UHitCapsuleComponent : public UCapsuleComponent, public IHitComponent
{
	GENERATED_BODY()

public:

	UHitCapsuleComponent(const FObjectInitializer& ObjectInitializer);

	void SetHitDimageMultiplier(float value);
	void SetThickness(int thickness);

	virtual FVector GetWorldLocation() override;
	virtual float GetHitDamageMultiplier() override;
	virtual int GetThickness() override;
	virtual int GetSpatterBackDistance() override;
	virtual void SetModularComponent(UModularSkeletonComponent* comp) override;
	virtual UModularSkeletonComponent* GetModularComponent() override;
	virtual void DamageModularComponent(int amount, FVector direction) override;

	virtual void AddEquipmentAsModularChild(AEquipment* equipment) override;
private:

	UPROPERTY() float DamageMultiplier;
	UPROPERTY() int Thickness;

	UPROPERTY() UModularSkeletonComponent* ModularComponent;

};
