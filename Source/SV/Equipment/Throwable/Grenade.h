// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Equipment.h"
#include "Grenade.generated.h"

class UThrowTravelComponent;
class UCapsuleComponent;
class UThrowExplosionComponent;

/**
 * 
 */
UCLASS()
class SV_API AGrenade : public AEquipment
{
	GENERATED_BODY()

public:
	AGrenade(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* GrenadeMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UThrowTravelComponent* TravelComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UCapsuleComponent* CapsuleComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UThrowExplosionComponent* ExplosionComponent;
};
