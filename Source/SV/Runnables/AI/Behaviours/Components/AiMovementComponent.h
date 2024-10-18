// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AiMovementComponent.generated.h"


class ISvChar;

UCLASS()
class SV_API UAiMovementComponent : public UObject
{
	GENERATED_BODY()

public:
	bool AttemptToRouteToPossibleLocation(TScriptInterface<ISvChar> character, FVector possibleLocation, TArray<FVector>& finalLocations);

private:

	void FindPathPointsToLocation(FVector start, FVector end, TArray<FVector>& navPath);
	FVector GetPointBetweenVectors(FVector StartVector, FVector EndVector, float DistanceFromStart);
};
