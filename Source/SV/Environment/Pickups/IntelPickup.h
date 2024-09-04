// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IntelPickup.generated.h"

class UPickupMeshComponent;
class UPickupDetailsComponent;
class UPickupIndicatorLink;

UCLASS()
class SV_API AIntelPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIntelPickup();
	virtual void BeginPlay() override;
protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UPickupMeshComponent* PickupMeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UPickupDetailsComponent* PickupDetailsComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UPickupIndicatorLink* PickupIndicatorLink;
};
