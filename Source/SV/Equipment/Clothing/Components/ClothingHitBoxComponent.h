// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "../../../Interfaces/HitComponent.h"
#include "ClothingHitBoxComponent.generated.h"

class UModularSkeletonComponent;
/**
 *
 */
UCLASS()
class SV_API UClothingHitBoxComponent : public UBoxComponent, public IHitComponent
{
	GENERATED_BODY()

public:
	UClothingHitBoxComponent(const FObjectInitializer &ObjectInitializer);

	void SetHitDimageMultiplier(float value);
	void SetThickness(int thickness);

	virtual FVector GetWorldLocation() override;
	virtual float GetHitDamageMultiplier() override;
	virtual int GetThickness() override;
	virtual int GetSpatterBackDistance() override;

	virtual void SetModularComponent(UModularSkeletonComponent *comp) override;
	virtual UModularSkeletonComponent *GetModularComponent() override;
	virtual void DamageModularComponent(int amount, FVector direction) override;
	virtual bool GetIsPreferredHitLocation() override;
	virtual void SetIsPreferredHitLocation(bool val) override;
	virtual void AddEquipmentAsModularChild(AEquipment *equipment) override;
	virtual FVector GetHitLocation() override;
	virtual AActor *GetOwningActor() override;

	virtual UPrimitiveComponent *GetAsPrimitiveComponent() override;

protected:
	UPROPERTY()
	float DamageMultiplier;
	UPROPERTY()
	int Thickness;
	UPROPERTY()
	bool bIsPreferredTarget;

	UPROPERTY()
	UModularSkeletonComponent *ModularComponent;
};
