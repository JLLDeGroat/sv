// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RouteDataManager.generated.h"

/**
 * 
 */
UCLASS()
class SV_API URouteDataManager : public UObject
{
	GENERATED_BODY()

public:

	URouteDataManager();

	void SetCurrentRoute(TArray<FVector2D> arr);
	void SetCurrentOffshoots(TArray<FVector2D> arr);

	TArray<FVector2D> GetCurrentRoute();
	TArray<FVector2D> GetCurrentOffshoots();

	void SetCurrentLocationOnRoute(FVector2D loc);
	FVector2D GetCurrentLocationOnRoute();
private:

	UPROPERTY() TArray<FVector2D> CurrentRoute;
	UPROPERTY() TArray<FVector2D> Offshoots;
	UPROPERTY() FVector2D CurrentLocationOnRoute;
};
