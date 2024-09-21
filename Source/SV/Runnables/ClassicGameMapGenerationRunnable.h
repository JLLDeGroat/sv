// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseRunnable.h"
#include "../Data/FMapGridData.h"
#include "ClassicGameMapGenerationRunnable.generated.h"

class UNewGameOptionsWidget;

/**
 * 
 */
UCLASS()
class SV_API UClassicGameMapGenerationRunnable : public UBaseRunnable
{
	GENERATED_BODY()
public:

	virtual void ActivateThread() override;

	UClassicGameMapGenerationRunnable* InsertVariables(UNewGameOptionsWidget* widget, FMapGridData* mapGrid);

private:
	//Generating Base Routes
	void FindValidRoutes();
	void FindValidRoutesRecursive(TArray<FVector2D> currentRoute);

	UPROPERTY() FMapGridData CurrentMapGrid;
	UPROPERTY() UNewGameOptionsWidget* GameModeWidget;
	UPROPERTY() FVector2D Start;
	UPROPERTY() FVector2D End;
	UPROPERTY() TArray<FVector2D> ChosenPrimaryRoute;

	bool AlreadyInRoute(TArray<FVector2D> route, FVector2D loc);
	bool IsPossibleToReachEnd(TArray<FVector2D> current);
	bool IsAdjacentToNonPreviousLocations(TArray<FVector2D> previousRoute, FVector2D newLoc);

	//Generated Offshoots
	void FindValidOffshoots();
	void FindValidOffshootsRecursive(TArray<FVector2D> currentOffshoot, bool isInitial = false);
	
	bool ShouldGenerateOffShoot(int degradation = 5, int incrememtation = 2);
	bool IsPartOfMainRoute(FVector2D arr);
	bool HasReachedMaxOffshoots();
	bool AlreadyAnOffshoot(FVector2D vector);
	bool IsAdjacentToOtherOffshoots(FVector2D vector);

	void GenerateCrewMembers(int amount = 5);

	void GenerateWorldLocationData();
	void GenerateWorldLocationMissionsData();
	void GenerateBaseResourceAmounts();

	UPROPERTY() int BaseChance;
	UPROPERTY() int MaxOffshoots;
	UPROPERTY() TArray<FVector2D> Offshoots;
};
