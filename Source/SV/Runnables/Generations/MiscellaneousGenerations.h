// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseGenerations.h"
#include "MiscellaneousGenerations.generated.h"

/**
 *
 */
UCLASS()
class SV_API UMiscellaneousGenerations : public UBaseGenerations
{
	GENERATED_BODY()

public:
	virtual UBaseGenerations *Generate() override;

private:
	UPROPERTY()
	TArray<FVector> SpawningChestHighsLocations;
};
