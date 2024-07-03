// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "BulletCollisionComponent.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UBulletCollisionComponent : public UCapsuleComponent
{
	GENERATED_BODY()
	
public:

	UBulletCollisionComponent(const FObjectInitializer& ObjectInitializer);

protected:

	UFUNCTION() void Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION() void OnDestroyCallback();
	FTimerHandle DestroyTimer;

};
