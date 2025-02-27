// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FGridDataItem.generated.h"

/**
 *
 */

USTRUCT()
struct FGridDataItem
{
	GENERATED_BODY()
public:
	FGridDataItem()
	{
	}

	FGridDataItem(FVector location)
	{
		Location = location;
	}
	FGridDataItem(int x, int y, int z)
	{
		X = x;
		Y = y;
		Z = z;
		Location = FVector(X * 100, Y * 100, Z * 200);
		bIsTraversableSpace = false;
		bIsVoidSpace = false;
		bIsRoad = false;
		bIsPrefab = false;
		bIsBuffer = false;
		bIsWall = false;
		bIsDebris = false;
		bIsEndZone = false;
		bIsStartZone = false;
	}

	FVector GetLocation() { return Location; }
	bool GetIsRoad() { return bIsRoad; }

	void SetLocation(FVector loc) { Location = loc; }
	void SetIsRoad() { bIsRoad = true; }

	bool GetIsPrefab() { return bIsPrefab; }
	void SetIsPrefab() { bIsPrefab = true; }

	bool GetIsBuffer() { return bIsBuffer; }
	void SetIsBuffer() { bIsBuffer = true; }

	bool GetIsVoid() { return bIsVoidSpace; }
	void SetIsVoid() { bIsVoidSpace = true; }

	bool GetIsWall() { return bIsWall; }
	void SetIsWall() { bIsWall = true; }

	bool GetIsDebris() { return bIsDebris; }
	void SetIsDebris() { bIsDebris = true; }

	bool GetIsEndZone() { return bIsEndZone; }
	void SetIsEndZone() { bIsEndZone = true; }

	bool GetIsStartZone() { return bIsStartZone; }
	void SetIsStartZone() { bIsStartZone = true; }

	bool GetIsPotentialWallSpace() { return !bIsPrefab && !bIsBuffer && !bIsRoad && !bIsEndZone && !bIsStartZone; }
	bool GetIsPotentialMiscItemSpace() { return !bIsPrefab && !bIsBuffer && !bIsRoad && !bIsEndZone && !bIsStartZone && !bIsWall && !bIsVoidSpace; }

protected:
	UPROPERTY()
	FVector Location;

	UPROPERTY()
	int X;
	UPROPERTY()
	int Y;
	UPROPERTY()
	int Z;

	UPROPERTY()
	bool bIsTraversableSpace;

	UPROPERTY()
	bool bIsVoidSpace;

	UPROPERTY()
	bool bIsRoad;

	UPROPERTY()
	bool bIsPrefab;

	UPROPERTY()
	bool bIsBuffer;

	UPROPERTY()
	bool bIsWall;

	UPROPERTY()
	bool bIsDebris;

	UPROPERTY()
	bool bIsEndZone;

	UPROPERTY()
	bool bIsStartZone;
};