// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridUtilities.generated.h"
/**
 * 
 */


UCLASS()
class SV_API UGridUtilities : public UObject
{
	GENERATED_BODY()
public:
	static FVector GetNormalisedGridLocation(FVector location);
	static TArray<FVector> GetGridLocationSteps(FVector start, FVector end);

	static FRotator FindLookAtRotation(FVector source, FVector target);

private:

	static float NormalizeGridValue(float loc);
};
