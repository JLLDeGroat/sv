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
#include "ClimbLadderComponent.h"
#include "../../Runnables/PostMovementRunnable.h"
#include "../../Environment/Components/VaultableComponent.h"
#include "../../Environment/Components/SkippableComponent.h"
#include "../../World/WorldGridItemActor.h"
#include "../../Environment/Traversals/Components/TraversalLocationsComponent.h"
#include "Algo/Sort.h"

// Sets default values for this component's properties
UGridMovementComponent::UGridMovementComponent(const FObjectInitializer &ObjectInitializer) : UAnimAccessComponent(ObjectInitializer)
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

void UGridMovementComponent::SetShouldPauseMovement(bool val)
{
	bShouldPauseMovement = val;
}

// Called every frame
void UGridMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MovementLocations.Num() > 0)
	{
		if (bPauseMovement)
			return;

		CurrentMovementType = EMovementType::MT_Horizontal;
		auto zDifference = MovementLocations[0].Z - GetOwner()->GetActorLocation().Z;
		if (zDifference > 200)
			CurrentMovementType = EMovementType::MT_Vertical_UP;
		else if (zDifference < -200)
			CurrentMovementType = EMovementType::MT_Vertical_DOWN;

		if (CurrentMovementType != EMovementType::MT_Horizontal)
		{
			auto laddClimb = GetOwner()->GetComponentByClass<UClimbLadderComponent>();
			if (!laddClimb)
				return UDebugMessages::LogError(this, "failed to get climb");

			laddClimb->StartClimbingLadder(CurrentMovementType, MovementLocations[0],
										   CurrentMovementType == EMovementType::MT_Vertical_DOWN ? EClimbType::CT_Down : EClimbType::CT_Up);

			SetComponentTickEnabled(false);
		}
		else
		{

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
			if (dist < 5)
			{
				MovementLocations.RemoveAt(0);
				USvUtilities::AttemptToStartStatUpdater(GetOwner(), EStatisticType::ST_MOVEMENT, 1);
				USvUtilities::AttemptToStartDirectorStatUpdater(GetOwner(), EDirectorStatType::DS_MOVEMENT, 1);
				USvUtilities::AttemptToStartDirectorStatUpdater(GetOwner(), EDirectorStatType::DS_NOISE, 1);
				if (bShouldPauseMovement)
				{
					bShouldPauseMovement = false; // revert this to false as to not hit next time
					AnimInstance->UpdateSpeed(0);
					bPauseMovement = true;
					GetWorld()->GetTimerManager().SetTimer(GridMovementDelayTimer, this, &UGridMovementComponent::OnGridMovementDelayTimerCallback, 1.5f, false);
				}
			}
		}
	}
	else
	{
		SetComponentTickEnabled(false);

		if (AnimInstance)
			AnimInstance->UpdateSpeed(0);

		PostMovementRunnable = NewObject<UPostMovementRunnable>()
								   ->InsertVariables(GetOwner())
								   ->Initialise(GetWorld())
								   ->Begin();
	}
}

void UGridMovementComponent::OnGridMovementDelayTimerCallback()
{
	bPauseMovement = false;
}

bool UGridMovementComponent::MoveAcrossGrid(TArray<FVector> movementLocs)
{
	if (movementLocs.Num() == 0)
	{
		UDebugMessages::LogError(this, "movementLocs were empty, breaking error");
		return false;
	}
	MovementLocations = movementLocs;
	AnimInstance->SetIsCrouching(false);
	SetComponentTickEnabled(true);
	return true;
}

void UGridMovementComponent::MovementLoop()
{
	if (MovementLocations.Num() == 0)
		return;
}

void UGridMovementComponent::MoveAcrossGridPostClimb()
{
	MovementLocations.RemoveAt(0);
	SetComponentTickEnabled(true);
}

bool UGridMovementComponent::HasFoundEnd()
{
	for (int i = 0; i < MovementData.Num(); i++)
	{
		if (MovementData[i].GetIsEnd())
			return true;
	}
	return false;
}

FMovementData *UGridMovementComponent::HasAnalysedGridItem(FVector startLocation)
{
	for (int i = 0; i < MovementData.Num(); i++)
		if (MovementData[i].GetStart() == startLocation)
			return &MovementData[i];

	return nullptr;
}

bool UGridMovementComponent::AlreadyInPrevious(FVector gridLocation, TArray<FVector> previous)
{
	for (int i = 0; i < previous.Num(); i++)
		if (previous[i] == gridLocation)
			return true;

	return false;
}

TArray<FVector> UGridMovementComponent::FindRoute(FVector start, FVector end, bool bisAI)
{
	MovementData.Empty();

	auto detailsComponent = GetOwner()->GetComponentByClass<UCharacterDetailsComponent>();

	if (detailsComponent->GetMovementPoints() > 0)
	{
		TArray<FVector> emptyPrevious;
		auto thisMovementDataIndex = GetMovementDataForGridItem(start, emptyPrevious, end);
		auto thisMovementData = &MovementData[thisMovementDataIndex];

		FindRouteRecursive(thisMovementData, end, bisAI);

		if (HasFoundEnd())
		{
			// UDebugMessages::LogDisplay(this, "found end");
			for (int i = 0; i < MovementData.Num(); i++)
			{
				if (MovementData[i].GetIsEnd())
				{
					TArray<FVector> FinalMovement = MovementData[i].GetPrevious();
					FinalMovement.Emplace(MovementData[i].GetStart());
					return FinalMovement;
				}
			}
		}
		// else UDebugMessages::LogError(this, "could not find end");
	}

	TArray<FVector> response;
	return response;
}

TArray<FVector> UGridMovementComponent::FindQuickestRoute(FVector start, FVector end, bool bisAI)
{
	TArray<FVector> VisitedNodes;
	TArray<FVector> BestPath;
	TArray<FVector> CostGrid;
	float BestCost = FLT_MAX;
	bAIRouteDecided = false;

	FindQuickestRouteRecursive(start, end, VisitedNodes, BestCost, BestPath);

	return BestPath;
}

void UGridMovementComponent::FindRouteRecursive(FMovementData *movementData, FVector desiredLocation, bool bisAI)
{

	if (!HasFoundEnd())
	{
		TArray<FVector> newPrevious = movementData->GetPrevious();
		newPrevious.Emplace(movementData->GetStart());

		auto detailsComponent = GetOwner()->GetComponentByClass<UCharacterDetailsComponent>();

		/*if (!detailsComponent || (newPrevious.Num() > detailsComponent->GetMovementPoints() + 1 && !bisAI)) {*/
		if (!detailsComponent || (newPrevious.Num() > detailsComponent->GetMovementPoints() && !bisAI))
		{
			return;
		}

		TArray<int> newConnectionIds;

		for (int i = 0; i < movementData->GetConnections().Num(); i++)
		{
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

		for (int i = 0; i < newConnectionIds.Num(); i++)
		{
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

TArray<FVector> UGridMovementComponent::FindQuickestRouteRecursive(FVector Current, FVector End, TArray<FVector> &VisitedNodes, float &BestCost, TArray<FVector> &BestPath)
{
	if (bAIRouteDecided)
		return BestPath;

	// if (Current == End)
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

			// only use one for testing
			bAIRouteDecided = true;
		}
		else
		{
			AIRouteIterations += 1;
			UDebugMessages::LogDisplay(this, "adding to ai iterations " + FString::SanitizeFloat(AIRouteIterations));
			if (AIRouteIterations > 5)
			{
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

	for (const FVector &Neighbor : locs)
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

int UGridMovementComponent::GetMovementDataForGridItem(FVector gridItem, TArray<FVector> previous, FVector end)
{
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
#pragma optimize("", off)
bool UGridMovementComponent::GetMovableAdjacentTiles(FVector start, TArray<FVector> &ValidAdjacentTiles, FVector orderByDistanceLoc, bool bIgnoreVaultables, bool bIgnoreTraversals, bool bIgnoreSkippables)
{
	TArray<FVector> unorderedLocations;
	USvUtilities::GetAdjacentGridTiles(start, unorderedLocations);

	TArray<FVector> adjacentTiles = unorderedLocations;
	if (orderByDistanceLoc != FVector::ZeroVector)
	{
		Algo::Sort(adjacentTiles, [&orderByDistanceLoc](const FVector &A, const FVector &B)
				   {
				// Calculate squared distance to avoid unnecessary square root operations
				float DistanceA = FVector::DistSquared(A, orderByDistanceLoc);
				float DistanceB = FVector::DistSquared(B, orderByDistanceLoc);

				// Return true if A is closer to the ReferencePoint than B
				return DistanceA < DistanceB; });
	}

	auto world = GetOwner()->GetWorld();

	for (int i = 0; i < adjacentTiles.Num(); i++)
	{
		FHitResult EnvironmentHit;
		world->LineTraceSingleByChannel(EnvironmentHit, start, adjacentTiles[i], USvUtilities::GetEnvironmentChannel());

		FHitResult EntityHit;
		FCollisionQueryParams EntityHitParams;
		EntityHitParams.AddIgnoredActor(GetOwner());
		world->LineTraceSingleByChannel(EntityHit, start, adjacentTiles[i], USvUtilities::GetClickableChannel(), EntityHitParams);

		FHitResult TraversalHit;
		FCollisionObjectQueryParams objectParams;
		objectParams.AddObjectTypesToQuery(USvUtilities::GetTraversalObjectChannel());
		world->LineTraceSingleByObjectType(TraversalHit, start, adjacentTiles[i], objectParams);

		FHitResult FloorHit;
		world->LineTraceSingleByChannel(FloorHit, adjacentTiles[i], adjacentTiles[i] - FVector(0, 0, 200), USvUtilities::GetFloorTargetChannel());

		if (TraversalHit.GetActor() && !bIgnoreTraversals)
		{
			auto traversalComp = TraversalHit.GetActor()->GetComponentByClass<UTraversalLocationsComponent>();

			if (!traversalComp)
				UDebugMessages::LogError(this, "TODO: Hit Traversal actor but has no traversalLocationsComponent");

			auto startLoc = traversalComp->GetStartGridLocation();
			auto endLoc = traversalComp->GetEndGridLocation();

			if (startLoc.X == start.X && startLoc.Y == start.Y)
			{
				ValidAdjacentTiles.Emplace(endLoc);
			}
			else if ((endLoc.X == start.X && endLoc.Y == start.Y))
			{
				ValidAdjacentTiles.Emplace(startLoc);
			}
		}

		if (FloorHit.bBlockingHit)
		{
			auto detailsComponent = GetOwner()->GetComponentByClass<UCharacterDetailsComponent>();
			auto hasVaultComponentAndCanVault = EnvironmentHit.GetActor() &&
												EnvironmentHit.GetActor()->GetComponentByClass<UVaultableComponent>() &&
												detailsComponent &&
												detailsComponent->GetCanVault();

			auto hasSkippableComponentAndCanSkip = EnvironmentHit.GetActor() &&
												   EnvironmentHit.GetActor()->GetComponentByClass<USkippableComponent>() &&
												   detailsComponent &&
												   detailsComponent->GetCanSkip();

			if ((!EnvironmentHit.bBlockingHit && !EntityHit.bBlockingHit) ||
				(EnvironmentHit.bBlockingHit && hasVaultComponentAndCanVault && !bIgnoreVaultables) ||
				(EnvironmentHit.bBlockingHit && hasSkippableComponentAndCanSkip && !bIgnoreSkippables))
			{
				if (hasSkippableComponentAndCanSkip)
				{
					// check that the next one over is also available, if not ignore if it is..
					// add the adjacent tile thats on the other side of the skipped tile
					EntityHitParams.AddIgnoredActor(EnvironmentHit.GetActor());
					auto lookat = UGridUtilities::FindLookAtRotation(start, adjacentTiles[i]);
					auto lookatVector = lookat.Vector() * 100;
					auto extraStep = adjacentTiles[i] + lookatVector;
					world->LineTraceSingleByChannel(EnvironmentHit, adjacentTiles[i], extraStep, USvUtilities::GetEnvironmentChannel(), EntityHitParams);

					if (EnvironmentHit.bBlockingHit)
					{
						if (EnvironmentHit.GetActor() &&
							EnvironmentHit.GetActor()->GetComponentByClass<USkippableComponent>())
							continue;
						else
							ValidAdjacentTiles.Emplace(extraStep);
					}
					else
						ValidAdjacentTiles.Emplace(extraStep);
				}
				else
					ValidAdjacentTiles.Emplace(adjacentTiles[i]);
			}
			// else UDebugMessages::LogError(this, "Invalid spot " + adjacentTiles[i].ToString());
		}
	}
	return !ValidAdjacentTiles.IsEmpty();
}
#pragma optimize("", on)
bool UGridMovementComponent::CanReachDestination(FVector location, FVector end, int steps)
{
	auto stepBuffer = 3;

	auto xmovement = location.X - end.X;
	if (xmovement < 0)
		xmovement *= -1;

	auto ymovement = location.Y - end.Y;
	if (ymovement < 0)
		ymovement *= -1;

	if ((ymovement + xmovement) / 100 <= (steps - stepBuffer))
	{
		return true;
	}
	else
		return false;
}

void UGridMovementComponent::ResetMovementSpeed()
{
	MovementSpeed = DefaultMovementSpeed;
	RotationSpeed = DefaultRotationSpeed;
}
void UGridMovementComponent::UpdateMovementSpeed(float speed)
{
	MovementSpeed = speed;
	RotationSpeed = speed / 2;
}

void UGridMovementComponent::SetMovementForOverwatchResponse()
{
	MovementSpeed = MovementSpeed / 100;
	RotationSpeed = RotationSpeed / 100;
	AnimInstance->UpdateAnimPlayRate(.05);
}
void UGridMovementComponent::ResetMovementAndAnimPlayBack()
{
	MovementSpeed = DefaultMovementSpeed;
	RotationSpeed = DefaultRotationSpeed;
	AnimInstance->UpdateAnimPlayRate(1);
}

void UGridMovementComponent::PostMovementCrouch()
{
	AnimInstance->SetIsCrouching(true);
}