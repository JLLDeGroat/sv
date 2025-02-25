// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseGenerations.h"
#include "BuildingGeneration.generated.h"

UCLASS()
class SV_API UBuildingGeneration : public UBaseGenerations
{
	GENERATED_BODY()

public:
	UBuildingGeneration();

	virtual UBaseGenerations *Generate() override;

	UBuildingGeneration *SetAmountToGenerate(int amount);

	TArray<FVector> GetDoorLocations();
private:
	FString GeneratePrefabTemplateLocations();

	UPROPERTY()
	TArray<FVector> DoorLocations;
	UPROPERTY()
	TArray<FVector> ThisBuildingsDoors;

	
	UPROPERTY()
	int AmountToGenerate;
	UPROPERTY()
	int AmountGenerated;

};
