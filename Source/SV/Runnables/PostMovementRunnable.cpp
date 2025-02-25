// Fill out your copyright notice in the Description page of Project Settings.

#include "PostMovementRunnable.h"
#include "../Utilities/SvUtilities.h"
#include "../Utilities/GridUtilities.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Environment/Components/VaultableComponent.h"
#include "../Environment/Components/SkippableComponent.h"
#include "../Characters/Components/GridMovementComponent.h"
#include "../Characters/Components/CharacterDetailsComponent.h"
#include "../Characters/Components/AIComponent.h"
#include "../Characters/Components/ActionsComponent.h"
#include "../Characters/Components/TargetingComponent.h"
#include "../Characters/Components/FogHandlerComponent.h"
#include "../Delegates/HudDelegates.h"
#include "../Delegates/AIDelegates.h"
#include "../Delegates/GameplayDelegates.h"
#include "DrawDebugHelpers.h"
#include "../Environment/Fog/Components/FogSectionComponent.h"

void UPostMovementRunnable::ActivateThread()
{

	auto gridLocation = UGridUtilities::GetNormalisedGridLocation(MovedActor->GetActorLocation());
	gridLocation.Z -= 50;
	TArray<FVector> adjacentLocations;
	USvUtilities::GetAdjacentGridTiles(gridLocation, adjacentLocations);

	for (int i = 0; i < adjacentLocations.Num(); i++)
	{
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, gridLocation, adjacentLocations[i], USvUtilities::GetEnvironmentChannel());
		// DrawDebugLine(GetWorld(), gridLocation, adjacentLocations[i], FColor::Red, true, 60, 0, 5);

		if (Hit.GetActor())
		{
			UDebugMessages::LogDisplay(this, "found actor " + Hit.GetActor()->GetName() + " comp " + Hit.GetComponent()->GetName());

			if (Hit.GetActor()->GetComponentByClass<UVaultableComponent>() ||
				Hit.GetActor()->GetComponentByClass<USkippableComponent>())
			{
				auto movementComponent = MovedActor->GetComponentByClass<UGridMovementComponent>();

				if (movementComponent)
					movementComponent->PostMovementCrouch();
			}
		}
	}
	auto targeting = MovedActor->GetComponentByClass<UTargetingComponent>();
	auto details = MovedActor->GetComponentByClass<UCharacterDetailsComponent>();
	if (targeting && details && details->GetCharacterControl() == ECharacterControl::CC_Player)
	{
		auto hudDelegates = UHudDelegates::GetInstance();
		if (!hudDelegates)
			return UDebugMessages::LogError(this, "failed to get hudDelegates");

		auto actionsComponent = MovedActor->GetComponentByClass<UActionsComponent>();

		if (!actionsComponent)
			return UDebugMessages::LogError(this, "failed to get actions component");

		auto gamePlayDelegates = UGameplayDelegates::GetInstance();
		if (!gamePlayDelegates)
			return UDebugMessages::LogError(this, "failed to get gameplay delegates");

		auto world = GetWorld();
		FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
			[hudDelegates, actionsComponent, gamePlayDelegates, targeting, world]
			{
				targeting->DetermineTargetData();
				actionsComponent->SendActionsToUI();
				hudDelegates->_RefreshCharacterDetailsWidget.Broadcast();
				gamePlayDelegates->_ChangeControlLimits.Broadcast(EControlLimit::CL_NONE);

				if (world)
					USvUtilities::AttemptToStartWinLossChecker(world); },
			TStatId(), nullptr, ENamedThreads::GameThread);
	}

	if (MovedActor->GetComponentByClass<UAIComponent>())
	{
		auto aiDelegates = UAIDelegates::GetInstance();
		if (!aiDelegates)
			return UDebugMessages::LogError(this, "failed to get ai delegates");

		FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
			[aiDelegates]
			{
				aiDelegates->_AICharacterFinishedBehaviour.Broadcast();
			},
			TStatId(), nullptr, ENamedThreads::GameThread);
	}

	auto fogComponent = MovedActor->GetComponentByClass<UFogHandlerComponent>();
	TArray<FVector> validAdjacentTiles;
	USvUtilities::GetAdjacentTilesForFogCalculation(MovedActor, validAdjacentTiles);
	validAdjacentTiles.Emplace(MovedActor->GetActorLocation());

	if (fogComponent)
	{
		auto fogSections = fogComponent->GetFogSectionComponents();

		for (int i = 0; i < fogSections.Num(); i++)
		{
			auto fogSectionToChange = fogSections[i];
			TArray<FVector> locationsToCheckFrom;

			for (int x = 0; x < validAdjacentTiles.Num(); x++)
				if (FVector::Dist(fogSectionToChange->GetComponentLocation(), validAdjacentTiles[x]) <= fogComponent->GetScaledSphereRadius())
					locationsToCheckFrom.Emplace(validAdjacentTiles[x]);

			for (int j = 0; j < locationsToCheckFrom.Num(); j++)
			{
				if (fogComponent->ShouldRemoveFog(fogSectionToChange, locationsToCheckFrom[j]))
				{
					FGraphEventRef routeTask = FFunctionGraphTask::CreateAndDispatchWhenReady([fogSectionToChange]
																							  { fogSectionToChange->SetAsNoFog(); }, TStatId(), nullptr, ENamedThreads::GameThread);
					break;
				}
			}
		}
	}
}

UPostMovementRunnable *UPostMovementRunnable::InsertVariables(AActor *movementActor)
{
	MovedActor = movementActor;
	return this;
}