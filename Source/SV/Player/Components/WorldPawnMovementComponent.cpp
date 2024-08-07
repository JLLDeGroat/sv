// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldPawnMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "../../Utilities/GridUtilities.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Instance/SvGameInstance.h"
#include "../../Instance/Managers/CurrentGameDataManager.h"
#include "../../Instance/Managers/RouteDataManager.h"
#include "../../GameModes/WorldGameMode.h"
#include "../../GameModes/WorldManagers/WorldDirectionManager.h"
#include "../../Delegates/WorldDelegates.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

// Sets default values for this component's properties
UWorldPawnMovementComponent::UWorldPawnMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldPawnMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	SetComponentTickEnabled(false);
}


// Called every frame
void UWorldPawnMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto currentLocation = GetOwner()->GetActorLocation();
	FVector newLocation = currentLocation;
	FRotator newRotation = GetOwner()->GetActorRotation();
	FRotator requiredRotation = newRotation;

	newLocation = UKismetMathLibrary::VInterpTo_Constant(GetOwner()->GetActorLocation(), MovementLocation, DeltaTime, 300);
	requiredRotation = UGridUtilities::FindLookAtRotation(newLocation, MovementLocation);
	newRotation = UKismetMathLibrary::RInterpTo_Constant(GetOwner()->GetActorRotation(), requiredRotation, DeltaTime, 500);
	newRotation.Pitch = GetOwner()->GetActorRotation().Pitch;
	newRotation.Roll = GetOwner()->GetActorRotation().Pitch;

	GetOwner()->SetActorLocation(newLocation);
	GetOwner()->SetActorRotation(newRotation);

	if (FVector::Dist(GetOwner()->GetActorLocation(), MovementLocation) < 5) {
		SetComponentTickEnabled(false);
		auto newGridLocation = UGridUtilities::GetRouteLocationFrom3DLoc(MovementLocation, true);
		auto gameInstance = USvUtilities::GetGameInstance(GetWorld());

		if (!gameInstance->GetCurrentGameDataManager() || !gameInstance->GetCurrentGameDataManager()->GetCurrentGameData())
			return UDebugMessages::LogError(this, "failed to get current game data manager");

		auto currentGameData = gameInstance->GetCurrentGameDataManager()->GetCurrentGameData();
		auto worldData = currentGameData->GetWorldData();

		worldData->SetNewLocationAsCurrent(newGridLocation);

		auto routeData = gameInstance->GetRouteDataManager();
		routeData->SetCurrentLocationOnRoute(newGridLocation);

		if (worldData->GetCurrentLocation()->GetMissionDetails()->GetIsCompleted()) {
			auto gameMode = GetWorld()->GetAuthGameMode<AWorldGameMode>();
			auto directionManager = gameMode->GetComponentByClass<UWorldDirectionManager>();
			if (!directionManager)
				return UDebugMessages::LogError(this, "failed to get direction manager");

			directionManager->GenerateDirections(newGridLocation);
		}

		auto worldDelegates = UWorldDelegates::GetInstance();
		if (!worldDelegates)
			return UDebugMessages::LogError(this, "failed to get world delegates");

		worldDelegates->_OnWorldMovementComplete.Broadcast(newGridLocation);
	}
}


void UWorldPawnMovementComponent::MoveToNewLocation(FVector gridSpot) {
	SetComponentTickEnabled(true);
	MovementLocation = gridSpot;
}
