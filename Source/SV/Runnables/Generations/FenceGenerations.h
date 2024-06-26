// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseGenerations.h"
#include "FenceGenerations.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UFenceGenerations : public UBaseGenerations
{
	GENERATED_BODY()

public:

	virtual UBaseGenerations* Generate() override;

protected:

	TArray<FVector> GenerateRequiredLocations();
	
	void BuildFence(FVector loc, FRotator rot = FRotator::ZeroRotator);
	void BuildPlot(FVector loc);
};
