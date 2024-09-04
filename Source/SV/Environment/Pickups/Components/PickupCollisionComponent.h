// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "PickupCollisionComponent.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UPickupCollisionComponent : public UBoxComponent
{
	GENERATED_BODY()

public:

	UPickupCollisionComponent();
	virtual void BeginPlay() override;

protected:
	UFUNCTION() void Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION() void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
