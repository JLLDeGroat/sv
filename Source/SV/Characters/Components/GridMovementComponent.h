// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/AnimAccessComponent.h"
#include "GridMovementComponent.generated.h"

class UBaseRunnable;
class UCharAnimInstance;

USTRUCT()
struct FMovementData
{
	GENERATED_BODY()

public:
	FMovementData(FVector start, TArray<FVector> previous) {
		Start = start;
		Previous = previous;
		IsEnd = false;
	}
	FMovementData(FVector start) {
		Start = start;
		IsEnd = false;
	}
	FMovementData() {
		Start = FVector::ZeroVector;
		IsEnd = false;
	}
	FMovementData(FMovementData* mvData) {
		Start = mvData->GetStart();
		Previous = mvData->GetPrevious();

		if (mvData->GetIsEnd())
			IsEnd = true;

		Connections = mvData->GetConnections();
	}

	void AddConnection(FVector connection) { Connections.Emplace(connection); }
	TArray<FVector> GetConnections() { return Connections; }

	void SetIsEnd() { IsEnd = true; }
	bool GetIsEnd() { return IsEnd; }

	TArray<FVector> GetPrevious() { return Previous; }

	FVector GetStart() { return Start; }
protected:

	UPROPERTY() FVector Start;
	UPROPERTY() TArray<FVector> Connections;

	UPROPERTY() TArray<FVector> Previous;

	UPROPERTY() bool IsEnd;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UGridMovementComponent : public UAnimAccessComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGridMovementComponent(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool MoveAcrossGrid(TArray<FVector> movementLocs);
	void MovementLoop();

	TArray<FVector> FindRoute(FVector start, FVector end, bool bisAI = false);
	TArray<FVector> FindQuickestRoute(FVector start, FVector end, bool bisAI = false);

	bool GetMovableAdjacentTiles(FVector start, TArray<FVector>& ValidAdjacentTiles, FVector orderByDistanceLoc = FVector::ZeroVector, bool bIgnoreVaultables = false);

	void ResetMovementSpeed();
	void UpdateMovementSpeed(float speed);

	void PostMovementCrouch();

	void SetMovementForOverwatchResponse();
	void ResetMovementAndAnimPlayBack();
private:

	UPROPERTY() TArray<FVector> MovementLocations;

	void FindRouteRecursive(FMovementData* movementData, FVector desiredLocation, bool bisAI = false);
	TArray<FVector> FindQuickestRouteRecursive(FVector Current, FVector End, TArray<FVector>& VisitedNodes, float& BestCost, TArray<FVector>& BestPath);

	int GetMovementDataForGridItem(FVector gridItem, TArray<FVector> previous, FVector end);
	bool HasFoundEnd();
	FMovementData* HasAnalysedGridItem(FVector startLocation);
	bool AlreadyInPrevious(FVector gridLocation, TArray<FVector> previous);

	bool CanReachDestination(FVector location, FVector end, int steps);

	UPROPERTY() TArray<AActor*> TestActorList;
	UPROPERTY() TArray<FMovementData> MovementData;

	UPROPERTY() UBaseRunnable* PostMovementRunnable;

	UPROPERTY() float MovementSpeed;
	UPROPERTY() float RotationSpeed;
	UPROPERTY() float DefaultMovementSpeed;
	UPROPERTY() float DefaultRotationSpeed;

	UPROPERTY() int AIRouteIterations;
	UPROPERTY() bool bAIRouteDecided;
};
