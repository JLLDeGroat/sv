// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseGenerations.h"
#include "PlayerGeneration.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UPlayerGeneration : public UBaseGenerations
{
	GENERATED_BODY()

public:

	virtual UBaseGenerations* Generate() override;
	
};
