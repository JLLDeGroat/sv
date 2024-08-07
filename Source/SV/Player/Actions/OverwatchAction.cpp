// Fill out your copyright notice in the Description page of Project Settings.


#include "OverwatchAction.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Equipment/Utility/OverwatchArea.h"
#include "../../Interfaces/Selectable.h"
#include "../Managers/SelectionManager.h"
#include "../Managers/ControlManager.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Utilities/GridUtilities.h"
#include "../Components/PawnCameraComponent.h"
#include "../../Delegates/HudDelegates.h"

#include "../../Characters/Components/CharacterDetailsComponent.h"
#include "../../Equipment/Equipment.h"
#include "../../Equipment/Components/EquipmentDetailsComponent.h"
#include "../../Characters/Components/EquipmentComponent.h"

UOverwatchAction::UOverwatchAction(const FObjectInitializer& ObjectInitializer)
	:UBaseActionComponent(ObjectInitializer) {

	PrimaryComponentTick.bCanEverTick = true;

	ValidCameraStates.Emplace(ECameraState::CS_Control);
	ValidCameraStates.Emplace(ECameraState::CS_Overwatch);
}

void UOverwatchAction::DoAction() {
	UDebugMessages::LogDisplay(this, "doing overwatch");
	ResetActionEffects();

	auto controller = GetOwner<APlayerController>();
	auto pawn = controller->GetPawn();
	auto pawnCameraComponent = pawn->GetComponentByClass<UPawnCameraComponent>();

	if (!pawnCameraComponent)
		return UDebugMessages::LogError(this, "failed to get pawn camera component");

	if (!IsInValidCameraState(pawnCameraComponent->GetCurrentCameraState()))
		return;

	auto selected = SelectionManager->GetSelected();
	if (selected) {
		auto controlComponent = controller->GetComponentByClass<UControlManager>();
		if (!controlComponent)
			return UDebugMessages::LogError(this, "failed to get control component");

		if (pawnCameraComponent->GetCurrentCameraState() == ECameraState::CS_Control) {
			if (!OverwatchArea)
				OverwatchArea = GetWorld()->SpawnActor<AOverwatchArea>(FVector(0, 0, -1000), FRotator::ZeroRotator);

			auto actorOwner = selected->GetAsActor();
			OverwatchArea->SetOverWatchOwner(actorOwner);
			pawnCameraComponent->UpdateCameraState(ECameraState::CS_Overwatch,
				FVector::ZeroVector, FVector::ZeroVector, true);

			ControlManager->SetCanMouseDesignateSelectionDecal(false);
			SetComponentTickEnabled(true);
		}
		else if (pawnCameraComponent->GetCurrentCameraState() == ECameraState::CS_Overwatch) {
			if (OverwatchArea) {
				OverwatchArea->Destroy();
				OverwatchArea = nullptr;
			}
			pawnCameraComponent->UpdateCameraState(ECameraState::CS_Control,
				FVector::ZeroVector, FVector::ZeroVector, true);

			ControlManager->SetCanMouseDesignateSelectionDecal(true);
			SetComponentTickEnabled(false);
		}
	}
}

void UOverwatchAction::SetOverwatch() {
	SetComponentTickEnabled(false);

	auto selected = SelectionManager->GetSelected();
	if (selected) {
		auto hudDelegates = UHudDelegates::GetInstance();
		if (!hudDelegates)
			return UDebugMessages::LogError(this, "failed to get hud delegates");

		auto selectedActor = selected->GetAsActor();
		auto characterDetails = selectedActor->GetComponentByClass<UCharacterDetailsComponent>();
		if (!characterDetails)
			return UDebugMessages::LogError(this, "failed to get character details");

		auto equipmentComponent = selected->GetAsActor()->GetComponentByClass<UEquipmentComponent>();
		if (!equipmentComponent)
			return UDebugMessages::LogError(this, "failed to get equipment component");

		auto primary = equipmentComponent->GetPrimaryEquipment();

		if (primary) {
			auto equipmentDetails = primary->GetComponentByClass<UEquipmentDetailsComponent>();
			characterDetails->RemoveActionPoints(equipmentDetails->GetOverwatchApCost());
			characterDetails->RemoveMovementPoints(characterDetails->GetMovementPoints());
			//deselect user and overwatch too

			hudDelegates->_HideOrResetUIWidget.Broadcast();
			hudDelegates->_ResetCharacterTileWidget.Broadcast();
			OverwatchArea->SetupForCollision();
			OverwatchArea = nullptr;
		}
	}
}

void UOverwatchAction::BeginPlay() {
	Super::BeginPlay();
	SetComponentTickEnabled(false);
}
void UOverwatchAction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult Hit;
	GetOwner<APlayerController>()->GetHitResultUnderCursor(USvUtilities::GetFloorTargetChannel(), false, Hit);

	if (Hit.GetActor()) {
		auto overwatchOwnerLocation = OverwatchArea->GetOverwatchOwnerLocation();
		auto midLocation = (Hit.Location + overwatchOwnerLocation) / 2;
		OverwatchArea->SetActorLocation(midLocation);

		auto lookAtRot = UGridUtilities::FindLookAtRotation(overwatchOwnerLocation, Hit.Location);
		OverwatchArea->SetActorRotation(lookAtRot);

		float distance = FVector::Dist(overwatchOwnerLocation, Hit.Location);
		float vector = distance / 100;
		OverwatchArea->SetActorScale3D(FVector(vector, vector, 1));
	}
}