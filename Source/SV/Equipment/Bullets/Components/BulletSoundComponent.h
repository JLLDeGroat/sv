// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "BulletSoundComponent.generated.h"

/**
 *
 */
UCLASS()
class SV_API UBulletSoundComponent : public USphereComponent
{
	GENERATED_BODY()

public:

	UBulletSoundComponent(const FObjectInitializer& ObjectInitializer);

protected:

	UFUNCTION() void Overlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
