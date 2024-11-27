// Fill out your copyright notice in the Description page of Project Settings.


#include "GridMovementComponent.h"
#include "../BaseCharacter.h"
#include "../Controllers/CharacterAIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "../../Utilities/GridUtilities.h"
#include "../../Utilities/SvUtilities.h"
#include "../Anim/CharAnimInstance.h"
#include "TargetingComponent.h"
#include "CharacterDetailsComponent.h"
#include "../../Runnables/PostMovementRunnable.h"
#include "../../Environment/Components/VaultableComponent.h"
#include "../../World/WorldGridItemActor.h"
#include "Algo/Sort.h"

// Sets default values for this component's properties
UGridMovementComponent::UGridMovementComponent(const FObjectInitializer& ObjectInitializer) : UAnimAccessComponent(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	DefaultMovementSpeed = 350;
	DefaultRotationSpeed = 200;
	RotationSpeed = DefaultRotationSpeed;
	MovementSpeed = DefaultMovementSpeed;

}


// Called when the game starts
void UGridMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);
}


// Called every frame
void UGridMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MovementLocations.Num() > 0) {
		auto normalisedMovementLocation = MovementLocations[0];
		normalisedMovementLocation.Z = GetOwner()->GetActorLocation().Z;

		auto newLocation = UKismetMathLibrary::VInterpTo_Constant(GetOwner()->GetActorLocation(), normalisedMovementLocation, DeltaTime, MovementSpeed);

		GetOwner()->SetActorLocation(newLocation);

		if (AnimInstance)
			AnimInstance->UpdateSpeed(200);

		auto lookAtRot = UGridUtilities::FindLookAtRotation(newLocation, normalisedMovementLocation);
		lookAtRot.Pitch = GetOwner()->GetActorRotation().Pitch;
		lookAtRot.Roll = GetOwner()->GetActorRotation().Roll;
		auto newRotation = UKismetMathLibrary::RInterpTo_Constant(GetOwner()->GetActorRotation(), lookAtRot, DeltaTime, RotationSpeed);
		GetOwner()->SetActorRotation(newRotation);

		auto dist = FVector::Distance(GetOwner()->GetActorLocation(), normalisedMovementLocation);
		if (dist < 5) {
			MovementLocations.RemoveAt(0);
		}
	}
	else {
		SetComponentTickEnabled(false);

		if (AnimInstance)
			AnimInstance->UpdateSpeed(0);

		PostMovementRunnable = NewObject<UPostMovementRunnable>()
			->InsertVariables(GetOwner())
			->Initialise(GetWorld())
			->Begin();
	}
}

bool UGridMovementComponent::MoveAcrossGrid(TArray<FVector> movementLocs) {
	if (movementLocs.Num() == 0) {
		UDebugMessages::LogError(this, "movementLocs were empty, breaking error");
		return false;
	}
	MovementLocations = movementLocs;
	AnimInstance->SetIsCrouching(false);
	SetComponentTickEnabled(true);
	return true;
}

void UGridMovementComponent::MovementLoop() {
	if (MovementLocations.Num() == 0)
		return;
}

bool UGridMovementComponent::HasFoundEnd() {
	for (int i = 0; i < MovementData.Num(); i++) {
		if (MovementData[i].GetIsEnd()) return true;
	}
	return false;
}


FMovementData* UGridMovementComponent::HasAnalysedGridItem(FVector startLocation) {
	for (int i = 0; i < MovementData.Num(); i++)
		if (MovementData[i].GetStart() == startLocation)
			return &MovementData[i];

	return nullptr;
}

bool UGridMovementComponent::AlreadyInPrevious(FVector gridLocation, TArray<FVector> previous) {
	for (int i = 0; i < previous.Num(); i++)
		if (previous[i] == gridLocation)
			return true;

	return false;
}

TArray<FVector> UGridMovementComponent::FindRoute(FVector start, FVector end, bool bisAI) {
	MovementData.Empty();

	auto detailsComponent = GetOwner()->GetComponentByClass<UCharacterDetailsComponent>();

	if (detailsComponent->GetMovementPoints() > 0) {
		TArray<FVector> emptyPrevious;
		auto thisMovementDataIndex = GetMovementDataForGridItem(start, emptyPrevious, end);
		auto thisMovementData = &MovementData[thisMovementDataIndex];

		FindRouteRecursive(thisMovementData, end, bisAI);

		if (HasFoundEnd()) {
			//UDebugMessages::LogDisplay(this, "found end");
			for (int i = 0; i < MovementData.Num(); i++) {
				if (MovementData[i].GetIsEnd()) {
					TArray<FVector> FinalMovement = MovementData[i].GetPrevious();
					FinalMovement.Emplace(MovementData[i].GetStart());
					return FinalMovement;
				}
			}
		}
		//else UDebugMessages::LogError(this, "could not find end");
	}

	TArray<FVector> response;
	return response;
}

TArray<FVector> UGridMovementComponent::FindQuickestRoute(FVector start, FVector end, bool bisAI) {
	TArray<FVector> VisitedNodes;
	TArray<FVector> BestPath;
	TArray<FVector> CostGrid;
	float BestCost = FLT_MAX;
	bAIRouteDecided = false;

	FindQuickestRouteRecursive(start, end, VisitedNodes, BestCost, BestPath);

	return BestPath;
}

void UGridMovementComponent::FindRouteRecursive(FMovementData* movementData, FVector desiredLocation, bool bisAI) {

	if (!HasFoundEnd()) {
		TArray<FVector> newPrevious = movementData->GetPrevious();
		newPrevious.Emplace(movementData->GetStart());

		auto detailsComponent = GetOwner()->GetComponentByClass<UCharacterDetailsComponent>();

		/*if (!detailsComponent || (newPrevious.Num() > detailsComponent->GetMovementPoints() + 1 && !bisAI)) {*/
		if (!detailsComponent || (newPrevious.Num() > detailsComponent->GetMovementPoints() && !bisAI)) {
			return;
		}

		TArray<int> newConnectionIds;

		for (int i = 0; i < movementData->GetConnections().Num(); i++) {
			auto movementDataForGridIndex = GetMovementDataForGridItem(movementData->GetConnections()[i], newPrevious, desiredLocation);

			if (movementDataForGridIndex == -1)
				continue;

			newConnectionIds.Emplace(movementDataForGridIndex);
		}

		/*auto world = GetWorld();
		FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, movementData] {
			auto actor = world->SpawnActor<AWorldGridItemActor>(movementData->GetStart() - FVector(50, 50, 0), FRotator::ZeroRotator);
			actor->SetIsSpawn();
			actor->SetAutoDestroy();
			}, TStatId(), nullptr, ENamedThreads::GameThread);
		FPlatformProcess::Sleep(.25f);*/

		for (int i = 0; i < newConnectionIds.Num(); i++) {
			if (MovementData[newConnectionIds[i]].GetStart().X == desiredLocation.X &&
				MovementData[newConnectionIds[i]].GetStart().Y == desiredLocation.Y)
			{
				MovementData[newConnectionIds[i]].SetIsEnd();
				return;
			}

			if (MovementData[newConnectionIds[i]].GetConnections().Num() > 0)
				FindRouteRecursive(&MovementData[newConnectionIds[i]], desiredLocation, bisAI);
		}
	}
}

TArray<FVector> UGridMovementComponent::FindQuickestRouteRecursive(FVector Current, FVector End, TArray<FVector>& VisitedNodes, float& BestCost, TArray<FVector>& BestPath)
{
	if (bAIRouteDecided)
		return BestPath;

	//if (Current == End)
	if (Current.Equals(End))
	{
		// If we reached the end, calculate the cost
		float CurrentCost = VisitedNodes.Num();
		/*for (const FVector& Node : VisitedNodes)
		{
			int32 Index = Grid.Find(Node);
			if (Index != INDEX_NONE)
			{
				CurrentCost += CostGrid[Index];
			}
		}*/

		// If the current path cost is better, update the best path
		if (CurrentCost < BestCost)
		{
			UDebugMessages::LogDisplay(this, "found new bestCost " + FString::SanitizeFloat(CurrentCost));
			AIRouteIterations = 0;
			BestCost = CurrentCost;
			BestPath = VisitedNodes;

			//only use one for testing
			bAIRouteDecided = true;
		}
		else {
			AIRouteIterations += 1;
			UDebugMessages::LogDisplay(this, "adding to ai iterations " + FString::SanitizeFloat(AIRouteIterations));
			if (AIRouteIterations > 5) {
				bAIRouteDecided = true;
			}
		}
		return BestPath;
	}

	/*auto world = GetWorld();
	FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([world, Current] {
		auto actor = world->SpawnActor<AWorldGridItemActor>(Current - FVector(50, 50, 0), FRotator::ZeroRotator);
		actor->SetIsSpawn();
		actor->SetAutoDestroy();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	FPlatformProcess::Sleep(.25f);*/

	// Mark the current node as visited
	VisitedNodes.Add(Current);

	// Explore neighboring nodes
	TArray<FVector> locs;
	GetMovableAdjacentTiles(Current, locs, End);

	for (const FVector& Neighbor : locs)
	{
		if (!VisitedNodes.Contains(Neighbor) &&
			VisitedNodes.Num() < 50 &&
			CanReachDestination(Neighbor, End, (50 - VisitedNodes.Num())) /*&& Grid.Contains(Neighbor)*/)
		{
			FindQuickestRouteRecursive(Neighbor, End, VisitedNodes, BestCost, BestPath);
		}
	}

	// Backtrack: remove the current node from the visited list
	VisitedNodes.Remove(Current);
	return BestPath;
}

int UGridMovementComponent::GetMovementDataForGridItem(FVector gridItem, TArray<FVector> previous, FVector end) {
	if (AlreadyInPrevious(gridItem, previous))
		return -1;

	auto alreadyAnalysed = HasAnalysedGridItem(gridItem);
	if (alreadyAnalysed)
		return -1;

	auto world = GetOwner()->GetWorld();

	auto currentMovement = FMovementData(gridItem, previous);

	TArray<FVector> locs;
	GetMovableAdjacentTiles(gridItem, locs, end);

	for (int i = 0; i < locs.Num(); i++)
		currentMovement.AddConnection(locs[i]);

	MovementData.Emplace(currentMovement);
	int response = MovementData.Num() - 1;
	return response;
}

bool UGridMovementComponent::GetMovableAdjacentTiles(FVector start, TArray<FVector>& ValidAdjacentTiles, FVector orderByDistanceLoc, bool bIgnoreVaultables) {
	TArray<FVector> unorderedLocations;
	USvUtilities::GetAdjacentGridTiles(start, unorderedLocations);

	TArray<FVector> adjacentTiles = unorderedLocations;
	if (orderByDistanceLoc != FVector::ZeroVector) {
		Algo::Sort(adjacentTiles, [&orderByDistanceLoc](const FVector& A, const FVector& B)
			{
				// Calculate squared distance to avoid unnecessary square root operations
				float DistanceA = FVector::DistSquared(A, orderByDistanceLoc);
				float DistanceB = FVector::DistSquared(B, orderByDistanceLoc);

				// Return true if A is closer to the ReferencePoint than B
				return DistanceA < DistanceB;
			});
	}
	/*if (orderByDistanceLoc != FVector::ZeroVector) {
		adjacentTiles.Empty();
		while (unorderedLocations.Num() > 0) {
			float minDistance = -1;
			FVector loc = FVector::ZeroVector;
			auto index = -1;
			for (int i = 0; i < unorderedLocations.Num(); i++) {
				auto distance = FVector::Dist(unorderedLocations[i], orderByDistanceLoc);
				if (loc == FVector::ZeroVector || distance < minDistance) {
					minDistance = distance;
					loc = unorderedLocations[i];
					index = i;
				}
			}

			adjacentTiles.Emplace(unorderedLocations[index]);
			unorderedLocations.RemoveAt(index);
		}
	}*/

	auto world = GetOwner()->GetWorld();

	for (int i = 0; i < adjacentTiles.Num(); i++) {
		FHitResult EnvironmentHit;
		world->LineTraceSingleByChannel(EnvironmentHit, start, adjacentTiles[i], USvUtilities::GetEnvironmentChannel());

		FHitResult EntityHit;
		FCollisionQueryParams EntityHitParams;
		EntityHitParams.AddIgnoredActor(GetOwner());
		world->LineTraceSingleByChannel(EntityHit, start, adjacentTiles[i], USvUtilities::GetClickableChannel(), EntityHitParams);


		auto detailsComponent = GetOwner()->GetComponentByClass<UCharacterDetailsComponent>();
		auto hasVaultComponentAndCanVault = EnvironmentHit.GetActor() && EnvironmentHit.GetActor()->GetComponentByClass<UVaultableComponent>()
			&& detailsComponent && detailsComponent->GetCanVault();

		if ((!EnvironmentHit.bBlockingHit && !EntityHit.bBlockingHit) ||
			(EnvironmentHit.bBlockingHit && hasVaultComponentAndCanVault && !bIgnoreVaultables))
		{
			ValidAdjacentTiles.Emplace(adjacentTiles[i]);
		}
		//else UDebugMessages::LogError(this, "Invalid spot " + adjacentTiles[i].ToString());
	}

	return !ValidAdjacentTiles.IsEmpty();
}

bool UGridMovementComponent::CanReachDestination(FVector location, FVector end, int steps) {
	auto stepBuffer = 3;

	auto xmovement = location.X - end.X;
	if (xmovement < 0) xmovement *= -1;

	auto ymovement = location.Y - end.Y;
	if (ymovement < 0) ymovement *= -1;

	if ((ymovement + xmovement) / 100 <= (steps - stepBuffer)) {
		return true;
	}
	else return false;
}

void UGridMovementComponent::ResetMovementSpeed() {
	MovementSpeed = DefaultMovementSpeed;
	RotationSpeed = DefaultRotationSpeed;
}
void UGridMovementComponent::UpdateMovementSpeed(float speed) {
	MovementSpeed = speed;
	RotationSpeed = speed / 2;
}

void UGridMovementComponent::SetMovementForOverwatchResponse() {
	MovementSpeed = MovementSpeed / 100;
	RotationSpeed = RotationSpeed / 100;
	AnimInstance->UpdateAnimPlayRate(.05);
}
void UGridMovementComponent::ResetMovementAndAnimPlayBack() {
	MovementSpeed = DefaultMovementSpeed;
	RotationSpeed = DefaultRotationSpeed;
	AnimInstance->UpdateAnimPlayRate(1);
}

void UGridMovementComponent::PostMovementCrouch() {
	AnimInstance->SetIsCrouching(true);
}