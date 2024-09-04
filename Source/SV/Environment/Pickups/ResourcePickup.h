// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EnvironmentActor.h"
#include "ResourcePickup.generated.h"

class UPickupMeshComponent;
class UBoxComponent;
class UPickupDetailsComponent;
/**
 * 
 */
UCLASS()
class SV_API AResourcePickup : public AEnvironmentActor
{
	GENERATED_BODY()
	
public:

	AResourcePickup(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;


protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UPickupMeshComponent* PickupMeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UBoxComponent* BoxComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UPickupDetailsComponent* PickupDetailsComponent;

};
