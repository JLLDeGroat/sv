// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/AnimAccessComponent.h"
#include "PickupResourceComponent.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UPickupResourceComponent : public UAnimAccessComponent
{
	GENERATED_BODY()
	
public:
	UPickupResourceComponent(const FObjectInitializer& ObjectInitializer);

	void StartPickup();
	void AssignPickup();

	bool HasNearbyPickupActors();

	void AddToPickupActors(AActor* pickupsActor);
	void RemovePickupActor(AActor* pickupsActor);

private:

	UPROPERTY() TArray<AActor*> NearbyPickupActors;
};
