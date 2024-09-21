// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetAction.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Enums/ECharacterEnums.h"
#include "../Managers/SelectionManager.h"
#include "../../Utilities/SvUtilities.h"
#include "../Components/PawnCameraComponent.h"
#include "Camera/CameraComponent.h"
#include "../../Characters/Components/EquipmentComponent.h"
#include "../../Characters/Components/TargetingComponent.h"
#include "../../Characters/Components/CharacterDetailsComponent.h"
#include "../../Characters/Components/AttackComponent.h"
#include "../../Characters/Components/EquipmentComponent.h"
#include "../../Equipment/Components/EquipmentDetailsComponent.h"
#include "../../Equipment/Equipment.h"
#include "../../Interfaces/SvChar.h"
#include "../../Interfaces/Selectable.h"
#include "../GamePlayerController.h"
#include "../../Delegates/HudDelegates.h"
#include "Components/SphereComponent.h"
#include "../Utility/TargetingIndicatorActor.h"

UTargetAction::UTargetAction(const FObjectInitializer& ObjectInitializer) :UBaseActionComponent(ObjectInitializer) {
	ValidCameraStates.Emplace(ECameraState::CS_Control);
	ValidCameraStates.Emplace(ECameraState::CS_GunTarget);

	PrimaryComponentTick.bCanEverTick = true;
}

void UTargetAction::BeginPlay() {
	Super::BeginPlay();
	SetComponentTickEnabled(false);
	if (!TargetingIndicator) {
		TargetingIndicator = GetWorld()->SpawnActor<ATargetingIndicatorActor>();
		TargetingIndicator->SetActorLocation(FVector(0, 0, -1000));
	}

	auto owner = GetOwner<AGamePlayerController>();
	auto pawn = owner->GetPawn();
	if (!PawnCameraComponent)
		PawnCameraComponent = pawn->GetComponentByClass<UCameraComponent>();
}

void UTargetAction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult hit;
	FVector targetLocation = FVector::ZeroVector;
	GetTargetLocation(hit, targetLocation, PawnCameraComponent);
	TargetingIndicator->SetActorLocation(targetLocation);

	auto gunAccuracy = USvUtilities::DetermineAccuracyInidicatorScale(PawnCameraComponent->GetComponentLocation(), targetLocation,
		CurrentTargetAccuracy, CurrentTargetAccuracyDecay, CurrentTargetBaseAccuracy);

	TargetingIndicator->SetActorScale3D(gunAccuracy);
}

void UTargetAction::DoAction() {
	if (IsWithinValidControlLimiter()) {
		ResetActionEffects();

		auto owner = GetOwner<AGamePlayerController>();
		auto pawn = owner->GetPawn();
		auto pawnCameraComponent = pawn->GetComponentByClass<UPawnCameraComponent>();

		if (!PawnCameraComponent)
			PawnCameraComponent = pawn->GetComponentByClass<UCameraComponent>();

		if (!IsInValidCameraState(pawnCameraComponent->GetCurrentCameraState()))
			return;

		auto selected = SelectionManager->GetSelected();

		auto hudDelegates = UHudDelegates::GetInstance();
		if (!hudDelegates)
			return UDebugMessages::LogError(this, "failed to get hud delegates, cannot do target action");

		// reset values
		if (pawnCameraComponent->GetCurrentCameraState() == ECameraState::CS_GunTarget) {
			pawnCameraComponent->UpdateCameraState(ECameraState::CS_Control);
			owner->SetMouseAsUi();
			hudDelegates->_AimTargetVisibility.Broadcast(false);
		}
		else if (selected) {
			auto actor = selected->GetAsActor();

			auto equipmentComponent = actor->GetComponentByClass<UEquipmentComponent>();
			auto detailsComponent = actor->GetComponentByClass<UCharacterDetailsComponent>();

			if (!equipmentComponent || !detailsComponent ||
				equipmentComponent->GetActionPointsNeededToUseEquipment() > detailsComponent->GetActionPoints())
				return UDebugMessages::LogError(this, "could not get equpiment or details component, or not enough action points. will not begin targeting");

			auto targetingComponent = actor->GetComponentByClass<UTargetingComponent>();
			if (targetingComponent) {
				auto currentTargetData = targetingComponent->GetCurrentMainTarget();
				if (!currentTargetData) {
					UDebugMessages::LogError(this, "no targeting data");
					return;
				}
				//assuming only one target data

				pawnCameraComponent->UpdateCameraState(ECameraState::CS_GunTarget, currentTargetData->GetShootCameraLocation(),
					currentTargetData->GetCharacter()->GetSelectableGridLocation());

				owner->SetMouseAsGame();

				SetCurrentEquipmentAccuracy(actor);

				//shows the hud on screen
				//hudDelegates->_AimTargetVisibility.Broadcast(true);
				SetComponentTickEnabled(true);
			}
		}
	}
}

float UTargetAction::GetTargetingIndicatorRadius() {
	return TargetingIndicator->GetSphereScaledRadius();
}

float UTargetAction::SetScaleAndGetTargetingRadius(FVector scale) {
	TargetingIndicator->SetActorScale3D(scale);
	return GetTargetingIndicatorRadius();
}

ATargetingIndicatorActor* UTargetAction::GetTargetingIndicatorActor() {
	return TargetingIndicator;
}

void UTargetAction::ResetTargetingActor() {
	SetComponentTickEnabled(false);
	TargetingIndicator->SetActorLocation(FVector(0, 0, 5000));
}

void UTargetAction::SetCurrentEquipmentAccuracy(AActor* actor) {
	CurrentTargetAccuracy = 0;
	CurrentTargetAccuracyDecay = 0;
	CurrentTargetBaseAccuracy = 0;

	auto equipmentComponent = actor->GetComponentByClass<UEquipmentComponent>();
	if (equipmentComponent) {
		auto targetingEquipment = equipmentComponent->GetPrimaryEquipment();
		auto equipmentDetails = targetingEquipment->GetComponentByClass<UEquipmentDetailsComponent>();
		if (equipmentComponent) {
			CurrentTargetAccuracyDecay = equipmentDetails->GetAccuracyDecay();
			CurrentTargetAccuracy = equipmentDetails->GetAccuracy();
			CurrentTargetBaseAccuracy = equipmentDetails->GetBaseAccuracy();
		}
	}
}