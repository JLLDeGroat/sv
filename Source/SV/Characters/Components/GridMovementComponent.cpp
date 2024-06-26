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

// Sets default values for this component's properties
UGridMovementComponent::UGridMovementComponent(const FObjectInitializer& ObjectInitializer) : UAnimAccessComponent(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	DefaultMovementSpeed = 350;
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

		if(AnimInstance) 
			AnimInstance->UpdateSpeed(200);

		auto lookAtRot = UGridUtilities::FindLookAtRotation(newLocation, normalisedMovementLocation);
		lookAtRot.Pitch = GetOwner()->GetActorRotation().Pitch;
		lookAtRot.Roll = GetOwner()->GetActorRotation().Roll;
		auto newRotation = UKismetMathLibrary::RInterpTo_Constant(GetOwner()->GetActorRotation(), lookAtRot, DeltaTime, 200);
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
		
		auto targeting = GetOwner()->GetComponentByClass<UTargetingComponent>();
		if (targeting) {
			targeting->DetermineTargetData();
		}

		PostMovementRunnable = NewObject<UPostMovementRunnable>()
			->InsertVariables(GetOwner())
			->Initialise(GetWorld())
			->Begin();
	}
}

void UGridMovementComponent::MoveAcrossGrid(TArray<FVector> movementLocs) {
	if (movementLocs.Num() == 0) return;

	MovementLocations = movementLocs;
	AnimInstance->SetIsCrouching(false);
	SetComponentTickEnabled(true);

	/*auto owner = GetOwner<ABaseCharacter>();
	if (owner) {
		auto result = owner->GetAIController()->MoveToLocation(MovementLocations[0], 1.0f);
		if (result == EPathFollowingRequestResult::RequestSuccessful) {
			MovementLocations.RemoveAt(0);
		}
		else {
			UDebugMessages::LogDisplay(this, "UGridMovementComponent::MoveAcrossGrid failed to move or is already there");
		}
	}*/
}

void UGridMovementComponent::MovementLoop() {
	if (MovementLocations.Num() == 0)
		return;

	/*auto owner = GetOwner<ABaseCharacter>();
	if (owner) {
		auto result = owner->GetAIController()->MoveToLocation(MovementLocations[0], 1.0f);
		if (result == EPathFollowingRequestResult::RequestSuccessful) {
			MovementLocations.RemoveAt(0);
		}
		else {
			UDebugMessages::LogDisplay(this, "UGridMovementComponent::MoveAcrossGrid failed to move or is already there");
		}
	}*/
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
			UDebugMessages::LogDisplay(this, "found end");
			for (int i = 0; i < MovementData.Num(); i++) {
				if (MovementData[i].GetIsEnd()) {
					TArray<FVector> FinalMovement = MovementData[i].GetPrevious();
					FinalMovement.Emplace(MovementData[i].GetStart());
					return FinalMovement;
				}
			}
		}
		else UDebugMessages::LogError(this, "could not find end");
	}

	TArray<FVector> response;
	return response;
}

void UGridMovementComponent::FindRouteRecursive(FMovementData* movementData, FVector desiredLocation, bool bisAI) {

	if (!HasFoundEnd()) {
		TArray<FVector> newPrevious = movementData->GetPrevious();
		newPrevious.Emplace(movementData->GetStart());

		auto detailsComponent = GetOwner()->GetComponentByClass<UCharacterDetailsComponent>();

		if (!detailsComponent || (newPrevious.Num() > detailsComponent->GetMovementPoints() + 1 && !bisAI)) {
			UDebugMessages::LogError(this, "failed to get details component or no movement points left whilst not being AI, cannot move");
			return;
		}

		TArray<int> newConnectionIds;

		for (int i = 0; i < movementData->GetConnections().Num(); i++) {
			auto movementDataForGridIndex = GetMovementDataForGridItem(movementData->GetConnections()[i], newPrevious, desiredLocation);

			if (movementDataForGridIndex == -1)
				continue;

			newConnectionIds.Emplace(movementDataForGridIndex);
		}

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

bool UGridMovementComponent::GetMovableAdjacentTiles(FVector start, TArray<FVector>& ValidAdjacentTiles, FVector orderByDistanceLoc) {
	TArray<FVector> unorderedLocations;
	USvUtilities::GetAdjacentGridTiles(start, unorderedLocations);

	TArray<FVector> adjacentTiles = unorderedLocations;

	if (orderByDistanceLoc != FVector::ZeroVector) {
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
	}

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

		bool testV = false;
		if (EnvironmentHit.bBlockingHit && !hasVaultComponentAndCanVault) {
			if (EnvironmentHit.GetActor()->GetComponentByClass<UVaultableComponent>()) {
				UDebugMessages::LogError(this, "why 2");
				testV = true;
			}
		}

		if ((!EnvironmentHit.bBlockingHit && !EntityHit.bBlockingHit) || 
			(EnvironmentHit.bBlockingHit && hasVaultComponentAndCanVault))
		{
			if (testV) {
				UDebugMessages::LogError(this, "hello");
			}
			ValidAdjacentTiles.Emplace(adjacentTiles[i]);
		}
	}

	return !ValidAdjacentTiles.IsEmpty();
}

void UGridMovementComponent::ResetMovementSpeed() {
	MovementSpeed = DefaultMovementSpeed;
}
void UGridMovementComponent::UpdateMovementSpeed(float speed) {
	MovementSpeed = speed;
}
void UGridMovementComponent::PostMovementCrouch() {
	AnimInstance->SetIsCrouching(true);
}