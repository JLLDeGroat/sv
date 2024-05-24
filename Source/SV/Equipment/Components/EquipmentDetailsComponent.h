// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentDetailsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API UEquipmentDetailsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentDetailsComponent();

	bool GetIsMelee() const;
	void SetIsMelee(bool val);

	bool GetIsRange() const;
	void SetIsRange(bool val);

	int GetBaseDamage() const;
	void SetBaseDamage(int amount);

	float GetCritMultiplier() const;
	void SetCritMultiplier(float amount);

	void SetApCost(int amount);
	int GetApCost() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY() bool bIsRange;
	UPROPERTY() bool bIsMelee;		

	UPROPERTY() int BaseDamage;
	UPROPERTY() float CritMultiplier;

	UPROPERTY() int ApCost;
};
