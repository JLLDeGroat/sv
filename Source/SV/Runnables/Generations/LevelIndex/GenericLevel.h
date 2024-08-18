// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseLevelIndex.h"
#include "GenericLevel.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UGenericLevel : public UBaseLevelIndex
{
	GENERATED_BODY()

public:

	virtual void GenerateLevel() override;
	
};
