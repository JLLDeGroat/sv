// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Base/BasePrefab.h"
#include "BuildingPrefab.generated.h"

UCLASS()
class SV_API ABuildingPrefab : public ABasePrefab
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuildingPrefab(const FObjectInitializer &ObjectInitializer);

	TArray<FVector> GetDoorLocations();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FVector> DoorLocations;
};
