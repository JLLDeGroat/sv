// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FGridDataItem.h"
#include "FGridData.generated.h"

/**
 *
 */
USTRUCT()
struct FGridData
{
	GENERATED_BODY()
public:
	FGridData()
	{
	}

	void AddDataItem(int x, int y, int z) { GridItems.Emplace(FGridDataItem(x, y, z)); }
	TArray<FGridDataItem> GetGridItems() { return GridItems; }

	FGridDataItem *GetDataItem(FVector location)
	{
		for (int i = 0; i < GridItems.Num(); i++)
			if (GridItems[i].GetLocation() == location)
				return &GridItems[i];

		return nullptr;
	}

protected:
	UPROPERTY()
	TArray<FGridDataItem> GridItems;
};
