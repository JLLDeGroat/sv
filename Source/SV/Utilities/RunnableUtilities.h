// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RunnableUtilities.generated.h"


struct FMapGridData;

/**
 * 
 */

UCLASS()
class SV_API URunnableUtilities : public UObject
{
	GENERATED_BODY()
public:
	
	static TArray<FVector2D> GetPassableAdjacentGridItems(FMapGridData* grid, FVector2D start, bool randomizeResult = true);

};