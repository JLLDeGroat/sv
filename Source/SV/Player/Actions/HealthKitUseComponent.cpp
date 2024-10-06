// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthKitUseComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Components/PawnCameraComponent.h"
#include "../../Interfaces/Selectable.h"
#include "../Managers/SelectionManager.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Utilities/GridUtilities.h"
#include "../../Environment/Indicators/HealthKitUseIndicator.h"
#include "../../Environment/Indicators/Components/IndicatorLinkComponent.h"
#include "../../Characters/Components/HealthKitsComponent.h"
#include "../../Environment/Indicators/HealthKitUseIndicator.h"

UHealthKitUseComponent::UHealthKitUseComponent(const FObjectInitializer& ObjectInitializer)
	: UBaseActionComponent(ObjectInitializer) {

	ValidCameraStates.Emplace(ECameraState::CS_Control);
}


void UHealthKitUseComponent::DoBasicHealth() {
	if (IsWithinValidControlLimiter()) {
		ResetActionEffects();

		auto pawn = GetOwningController()->GetPawn();
		auto pawnCamera = pawn->GetComponentByClass<UPawnCameraComponent>();

		if (!IsInValidCameraState(pawnCamera->GetCurrentCameraState()))
			return;

		auto selected = SelectionManager->GetSelected();
		if (!selected)
			return;

		auto selectedAsActor = selected->GetAsActor();

		pawnCamera->UpdateCameraState(ECameraState::CS_ToolUsage,
			FVector::ZeroVector, FVector::ZeroVector, true);

		auto healthKitComponent = selectedAsActor->GetComponentByClass<UHealthKitsComponent>();
		if (!healthKitComponent)
			return UDebugMessages::LogError(this, "failed to get health kit component");

		healthKitComponent->SetActiveHealthKit(EHealthKits::HK_Basic);

		TArray<AActor*> viableHealthKitTakers = FindAdjacentActors(selectedAsActor);
		viableHealthKitTakers.Emplace(selectedAsActor);

		for (int i = 0; i < viableHealthKitTakers.Num(); i++) {
			auto location = UGridUtilities::GetNormalisedGridLocation(viableHealthKitTakers[i]->GetActorLocation());
			location.Z = USvUtilities::GetTileElevation(location);

			auto kit = GetWorld()->SpawnActor<AHealthKitUseIndicator>(location, FRotator::ZeroRotator);
			if (kit) {
				HealthKitsIndicators.Emplace(kit);

				auto indicator = kit->GetComponentByClass<UIndicatorLinkComponent>();
				if (!indicator)
					return UDebugMessages::LogError(this, "failed to get indicator link component");
				indicator->SetToolIndicatingTo(viableHealthKitTakers[i]);
			}
			else UDebugMessages::LogError(this, "failed to spawn kit");
		}
	}
}
void UHealthKitUseComponent::DoLargeHealth() {
	if (IsWithinValidControlLimiter()) {
		ResetActionEffects();
		UDebugMessages::LogError(this, "METHOD NOT IMPLEMENTED DoLargeHealth");
	}
}
void UHealthKitUseComponent::DoAntidote() {
	if (IsWithinValidControlLimiter()) {
		ResetActionEffects();
		UDebugMessages::LogError(this, "METHOD NOT IMPLEMENTED DoAntidote");
	}
}
void UHealthKitUseComponent::DoFlameRetardent() {
	if (IsWithinValidControlLimiter()) {
		ResetActionEffects();
		UDebugMessages::LogError(this, "METHOD NOT IMPLEMENTED DoFlameRetardent");
	}
}

TArray<AActor*> UHealthKitUseComponent::FindAdjacentActors(AActor* actor) {
	auto middleLoc = UGridUtilities::GetNormalisedGridLocation(actor->GetActorLocation());
	TArray<AActor*> actors;
	TArray<FVector> adjacents;
	USvUtilities::GetAdjacentGridTiles(middleLoc, adjacents);

	for (int i = 0; i < adjacents.Num(); i++) {
		FHitResult EntityHit;
		FCollisionQueryParams EntityHitParams;
		EntityHitParams.AddIgnoredActor(actor);
		GetWorld()->LineTraceSingleByChannel(EntityHit, middleLoc, adjacents[i], USvUtilities::GetClickableChannel(), EntityHitParams);

		if (EntityHit.bBlockingHit)
			actors.Emplace(EntityHit.GetActor());
	}

	return actors;
}


void UHealthKitUseComponent::RemoveAllHealthKitIndicators() {
	for (int i = 0; i < HealthKitsIndicators.Num(); i++)
		if (HealthKitsIndicators[i])
			HealthKitsIndicators[i]->Destroy();

	HealthKitsIndicators.Empty();
}