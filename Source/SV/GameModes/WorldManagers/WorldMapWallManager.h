// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WorldMapWallManager.generated.h"
/**
 *
 */
UCLASS()
class SV_API UWorldMapWallManager : public UActorComponent
{
	GENERATED_BODY()
public:
	UWorldMapWallManager(const FObjectInitializer& ObjectInitializer);

	void SetFullRouteList(TArray<FVector2D> route);
	TArray<FVector2D> GetFullRouteList();

	void GenerateWalls();

	bool CanGoLeft(FVector2D loc, FVector2D& leftLoc);
	bool CanGoRight(FVector2D loc, FVector2D& rightLoc);
	bool CanGoUp(FVector2D loc, FVector2D& upLoc);
	bool CanGoDown(FVector2D loc, FVector2D& downLoc);

private:

	UPROPERTY() TArray<FVector2D> FullRouteList;

	bool ShouldBuildWallToLeft(FVector2D loc, FVector& outLoc, FRotator& outRot);
	bool ShouldBuildWallToRight(FVector2D loc, FVector& outLoc, FRotator& outRot);
	bool ShouldBuildWallToUp(FVector2D loc, FVector& outLoc, FRotator& outRot);
	bool ShouldBuildWallToBottom(FVector2D loc, FVector& outLoc, FRotator& outRot);

	FVector2D GetLeft(FVector2D loc);
	FVector2D GetRight(FVector2D loc);
	FVector2D GetUp(FVector2D loc);
	FVector2D GetDown(FVector2D loc);

	bool IsWithinRoute(FVector2D loc);
	FVector Make3DLocation(FVector2D loc);

	UPROPERTY() int Vector2DTo3DMultiplier;

	

};
