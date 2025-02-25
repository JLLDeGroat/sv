// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseGenerations.h"
#include "ExteriorGenerations.generated.h"

/**
 *
 */
UCLASS()
class SV_API UExteriorGenerations : public UBaseGenerations
{
	GENERATED_BODY()

public:
	virtual UBaseGenerations *Generate() override;
	UExteriorGenerations *SetAmountToGenerate(int amount);

	TArray<FVector> GetStartLocations();
protected:
	UPROPERTY()
	int AmountToGenerate;
	UPROPERTY()
	int AmountGenerated;

	UPROPERTY()
	TArray<FVector> StartLocations;

	FString GeneratePrefabTemplateLocations();
};
