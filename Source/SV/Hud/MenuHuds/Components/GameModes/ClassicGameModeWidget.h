// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../../../Data/FMapGridData.h"
#include "ClassicGameModeWidget.generated.h"


class UBaseRunnable;

/**
 * 
 */
UCLASS()
class SV_API UClassicGameModeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	void StartGeneration();

	void SetGridRoute(TArray<FVector2D> arr);
	void SetGridOffshoots(TArray<FVector2D> arr);

	FMapGridData* GetGridData();

protected:

	UFUNCTION(BlueprintImplementableEvent) void UpdateGridWithOffshoots(const TArray<FVector2D>& arr);
	UFUNCTION(BlueprintImplementableEvent) void SetGridDetails(const TArray<FVector2D>& arr, bool success, bool fail);

private:

	UPROPERTY() TArray<FVector2D> Route;
	UPROPERTY() TArray<FVector2D> Offshoots;

	UPROPERTY() FMapGridData CurrentMapGrid;
	UPROPERTY() UBaseRunnable* GenThread;

};
