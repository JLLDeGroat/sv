// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseLevelIndex.h"
#include "TwoBuildingsLevel.generated.h"

/**
 *
 */
UCLASS()
class SV_API UTwoBuildingsLevel : public UBaseLevelIndex
{
	GENERATED_BODY()

public:
	virtual void GenerateLevel() override;

private:
	void GenerateBuildings();
	void GenerateExteriors();

	void GenerateRoads();
};
