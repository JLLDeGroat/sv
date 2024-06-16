// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMapGridData.generated.h"

/**
 *
 */
USTRUCT()
struct FMapGridDataItem
{
	GENERATED_BODY()
public:
	FMapGridDataItem() {
		X = 0;
		Y = 0;
		bPassable = false;
		bIsStart = false;
		bIsEnd = false;
		bIsValid = false;
	}

	FMapGridDataItem(int x, int y, bool passable, bool isEnd, bool isStart) {
		X = x;
		Y = y;
		bPassable = passable;
		bIsStart = isStart;
		bIsEnd = isEnd;
		bIsValid = true;
	}

	int GetX() { return X; }
	int GetY() { return Y; }
	bool GetIsPassable() { return bPassable; }
	bool GetIsValid() { return bIsValid; }

	bool GetIsEnd() { return bIsEnd; }
	bool GetIsStart() { return bIsStart; }

	void SetIsStart() { bIsStart = true; }
	void SetIsEnd() { bIsEnd = true; }

	FVector2D GetAsVector() { return FVector2D(X, Y); }

protected:

	UPROPERTY() int X;
	UPROPERTY() int Y;
	UPROPERTY() bool bPassable;

	UPROPERTY() bool bIsStart;
	UPROPERTY() bool bIsEnd;

	UPROPERTY() bool bIsValid;
};


USTRUCT()
struct FMapGridData
{
	GENERATED_BODY()
public:
	FMapGridData() {

	}

	FMapGridData(int xAmount, int yAmount, bool randomStartAndEnd = true) {
		StartX = 0;
		StartY = 0;
		EndX = 0;
		EndY = 0;

		if (randomStartAndEnd) {
			StartX = FMath::RandRange(1, xAmount);
			StartY = FMath::RandRange(1, yAmount);
			EndX = FMath::RandRange(1, xAmount);
			EndY = FMath::RandRange(1, yAmount);

			while (AreAdjacent(StartX, StartY, EndX, EndY)) {
				StartX = FMath::RandRange(1, xAmount);
				StartY = FMath::RandRange(1, yAmount);
				EndX = FMath::RandRange(1, xAmount);
				EndY = FMath::RandRange(1, yAmount);
			}
		}

		for (int i = 0; i < xAmount; i++) {
			for (int x = 0; x < yAmount; x++) {
				auto xVal = i + 1;
				auto yVal = x + 1;
				if (randomStartAndEnd) {
					if (xVal == StartX && yVal == StartY) AddMapItem(xVal, yVal, true, false, true);
					else if (xVal == EndX && yVal == EndY) AddMapItem(xVal, yVal, true, true, false);
					else AddMapItem(xVal, yVal);
				}
				else
					AddMapItem(xVal, yVal);
			}
		}
	}



	void AddMapItem(int x, int y, bool passable = true, bool isEnd = false, bool isStart = false) {
		MapItems.Emplace(FMapGridDataItem(x, y, passable, isEnd, isStart));
	}

	bool GetIsPassable(int x, int y) {
		auto item = GetItem(x, y);

		return item.GetIsPassable() &&
			item.GetIsValid();
	}

	FMapGridDataItem GetItem(int x, int y) {
		for (int i = 0; i < MapItems.Num(); i++)
			if (MapItems[i].GetX() == x &&
				MapItems[i].GetY() == y)
				return MapItems[i];

		return FMapGridDataItem();
	}

	FMapGridDataItem* GetStart() {
		for (int i = 0; i < MapItems.Num(); i++) {
			if (MapItems[i].GetIsStart()) return &MapItems[i];
		}
		return nullptr;
	}

	FMapGridDataItem* GetEnd() {
		for (int i = 0; i < MapItems.Num(); i++) {
			if (MapItems[i].GetIsEnd()) return &MapItems[i];
		}
		return nullptr;
	}

protected:

	UPROPERTY() TArray<FMapGridDataItem> MapItems;


	UPROPERTY() int StartX;
	UPROPERTY() int EndX;
	UPROPERTY() int StartY;
	UPROPERTY() int EndY;

private:

	bool AreAdjacent(int sX, int sY, int eX, int eY) {
		if (sX == eX) {
			if (sY == eY || sY == eY + 1 || sY == eY - 1) return true;
		}
		else if (sY == eY) {
			if (sX == eX || sX == eX + 1 || sX == eX - 1) return true;
		}

		return false;
	}
};
