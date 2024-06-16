// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WorldDirectionManager.generated.h"

class ADirectionOptionActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SV_API UWorldDirectionManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWorldDirectionManager();

public:

	void SetFullRouteList(TArray<FVector2D> arr);
	void GenerateDirections(FVector2D currentLoc);

	void ClearDirections();

private:

	UPROPERTY() TArray<ADirectionOptionActor*> DirectionActors;
	UPROPERTY() TArray<FVector2D> FullRouteList;


	bool CanGoLeft(FVector2D loc, FVector2D& leftLoc);
	bool CanGoRight(FVector2D loc, FVector2D& rightLoc);
	bool CanGoUp(FVector2D loc, FVector2D& upLoc);
	bool CanGoDown(FVector2D loc, FVector2D& downLoc);
	FVector2D GetLeft(FVector2D loc);
	FVector2D GetRight(FVector2D loc);
	FVector2D GetUp(FVector2D loc);
	FVector2D GetDown(FVector2D loc);

	bool IsWithinRoute(FVector2D loc);
		
};
