// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnableUtilities.h"
#include "../Data/FMapGridData.h"

TArray<FVector2D> URunnableUtilities::GetPassableAdjacentGridItems(FMapGridData* grid, FVector2D start, bool randomizeResult) {
	auto left = FVector2D(start.X - 1, start.Y);
	auto right = FVector2D(start.X + 1, start.Y);
	auto up = FVector2D(start.X, start.Y - 1);
	auto down = FVector2D(start.X, start.Y + 1);

	TArray<FVector2D> result;

	if (grid->GetIsPassable(left.X, left.Y))	result.Emplace(left);
	if (grid->GetIsPassable(right.X, right.Y))  result.Emplace(right);
	if (grid->GetIsPassable(up.X, up.Y))		result.Emplace(up);
	if (grid->GetIsPassable(down.X, down.Y))	result.Emplace(down);

	if (result.Num() > 0 && randomizeResult)
	{
		int32 LastIndex = result.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
				result.Swap(i, Index);
		}
	}
	return result;
}