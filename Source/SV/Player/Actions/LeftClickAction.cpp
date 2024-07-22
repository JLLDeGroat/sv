// Fill out your copyright notice in the Description page of Project Settings.


#include "LeftClickAction.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Enums/ECharacterEnums.h"
#include "../Managers/SelectionManager.h"
#include "../Managers/ControlManager.h"
#include "../../Utilities/SvUtilities.h"
#include "../Components/PawnCameraComponent.h"
#include "../Components/CameraOverlapComponent.h"
#include "Camera/CameraComponent.h"
#include "../../Characters/Components/EquipmentComponent.h"
#include "../../Characters/Components/TargetingComponent.h"
#include "../../Characters/Components/CharacterDetailsComponent.h"
#include "../../Characters/Components/AttackComponent.h"
#include "../../Characters/Components/ThrowableComponent.h"
#include "../../Characters/Components/ActionsComponent.h"
#include "../../Interfaces/SvChar.h"
#include "../../Interfaces/Selectable.h"
#include "../../Equipment/Equipment.h"
#include "../../Equipment/Components/EquipmentDetailsComponent.h"
#include "../Components/WorldPawnMovementComponent.h"
#include "../../GameModes/WorldGameMode.h"
#include "../../GameModes/WorldManagers/WorldDirectionManager.h"
#include "../../Delegates/HudDelegates.h"
#include "TargetAction.h"
#include "OverwatchAction.h"

// Sets default values for this component's properties
ULeftClickAction::ULeftClickAction(const FObjectInitializer& ObjectInitializer) : UBaseActionComponent(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	ValidCameraStates.Emplace(ECameraState::CS_Control);
	ValidCameraStates.Emplace(ECameraState::CS_GunTarget);
	ValidCameraStates.Emplace(ECameraState::CS_ThrowTarget);
	ValidCameraStates.Emplace(ECameraState::CS_Overwatch);
	// ...
}


// Called when the game starts
void ULeftClickAction::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void ULeftClickAction::DoAction() {
	ResetActionEffects();

	auto controller = GetOwner<APlayerController>();
	auto pawn = controller->GetPawn();
	auto pawnCameraComponent = pawn->GetComponentByClass<UPawnCameraComponent>();
	auto pawnOverlapCameraComponent = pawn->GetComponentByClass<UCameraOverlapComponent>();
	auto pawnCamera = pawn->GetComponentByClass<UCameraComponent>();

	if (!IsInValidCameraState(pawnCameraComponent->GetCurrentCameraState()))
		return;

	FHitResult Hit;
	controller->GetHitResultUnderCursor(USvUtilities::GetClickableChannel(), false, Hit);

	if (!pawn || !pawnCameraComponent || !pawnCamera)
		return UDebugMessages::LogError(this, "could not get pawn, pawn camera or pawn camara component, stopping Action");

	auto hudDelegates = UHudDelegates::GetInstance();
	if (!hudDelegates)
		return UDebugMessages::LogError(this, "failed to get hud delegates, cannot do left click action");

	if (pawnCameraComponent->GetCurrentCameraState() == ECameraState::CS_GunTarget) {
		auto targetLocation = FVector::ZeroVector;
		FHitResult TestHit;
		GetTargetLocation(TestHit, targetLocation, pawnCamera);

		pawnOverlapCameraComponent->ShrinkOverlapComponent();
		//DrawDebugLine(GetWorld(), pawnCamera->GetComponentLocation(), targetLocation, FColor::Cyan, false, 60.0f, 0, 5);

		hudDelegates->_AimTargetVisibility.Broadcast(false);

		auto selected = SelectionManager->GetSelected();
		auto actor = selected->GetAsActor();
		auto targetingComponent = actor->GetComponentByClass<UTargetingComponent>();
		if (selected && targetingComponent) {
			auto currentTargetData = targetingComponent->GetCurrentMainTarget();

			auto equipmentComponent = actor->GetComponentByClass<UEquipmentComponent>();
			auto detailsComponent = actor->GetComponentByClass<UCharacterDetailsComponent>();

			auto equipment = equipmentComponent->GetPrimaryEquipment();

			if (!equipmentComponent || !detailsComponent || !equipment)
				return UDebugMessages::LogError(this, "could not get equipment or details component, will not begin try attack");

			auto equipmentDetails = equipment->GetComponentByClass<UEquipmentDetailsComponent>();

			if (!equipmentDetails)
				return UDebugMessages::LogError(this, "could not get equipment details component");

			equipmentDetails->RemoveFromRounds(1);
			detailsComponent->RemoveActionPoints(equipmentComponent->GetActionPointsNeededToUseEquipment());

			auto attackComponent = selected->GetAsActor()->GetComponentByClass<UAttackComponent>();

			auto targetAction = controller->GetComponentByClass<UTargetAction>();
			if (!targetAction)
				return UDebugMessages::LogError(this, "no targeting action, failed");

			attackComponent->TryAttackLocation(currentTargetData->GetShootLocation(), targetLocation, targetAction->GetTargetingIndicatorRadius());
			pawnCameraComponent->DoCinematicAttackCameraMovement(selected->GetAsActor(), currentTargetData->GetCharacter()->GetAsActor());
		}
	}
	else if (pawnCameraComponent->GetCurrentCameraState() == ECameraState::CS_ThrowTarget) {
		auto selected = SelectionManager->GetSelected();
		auto actor = selected->GetAsActor();

		auto grenadeDestinationLocation = ControlManager->GetGrenadeIndicatorActorLocation();
		auto throwableComponent = selected->GetAsActor()->GetComponentByClass<UThrowableComponent>();

		if (throwableComponent)
			throwableComponent->ThrowAtLocation(grenadeDestinationLocation);

		ControlManager->SetCanMouseDesignateExplosionRadiusActor(false);

		pawnCameraComponent->UpdateCameraState(ECameraState::CS_Throw, FVector::ZeroVector, FVector::ZeroVector, true);
	}
	else if (pawnCameraComponent->GetCurrentCameraState() == ECameraState::CS_Overwatch) {
		auto overwatchAction = controller->GetComponentByClass<UOverwatchAction>();
		if (!overwatchAction)
			return UDebugMessages::LogError(this, "failed to get overwatch action");

		overwatchAction->SetOverwatch();
		pawnCameraComponent->UpdateCameraState(ECameraState::CS_Control, FVector::ZeroVector, FVector::ZeroVector, true);
	}
	else {
		if (Hit.GetActor() && SelectionManager->TrySetSelected(Hit.GetActor())) {
			SelectionManager->GetSelected()->TryVisualiseTargets();

			auto actionsComponent = Hit.GetActor()->GetComponentByClass<UActionsComponent>();

			if (!actionsComponent)
				return UDebugMessages::LogError(this, "failed to get actions component");

			actionsComponent->SendActionsToUI();
			hudDelegates->_CheckCharacterTileIsActive.Broadcast(Hit.GetActor());
		}
		else {
			hudDelegates->_HideOrResetUIWidget.Broadcast();
			hudDelegates->_ResetCharacterTileWidget.Broadcast();
		}
	}
}

void ULeftClickAction::DoWorldAction() {
	auto controller = GetOwner<APlayerController>();
	auto pawn = controller->GetPawn();

	FHitResult Hit;
	controller->GetHitResultUnderCursor(USvUtilities::GetWorldSelectChannel(), false, Hit);

	if (Hit.GetActor()) {
		auto movementComp = pawn->GetComponentByClass<UWorldPawnMovementComponent>();
		auto getClickedLocation = Hit.GetActor()->GetActorLocation();
		movementComp->MoveToNewLocation(getClickedLocation);

		auto gameMode = GetWorld()->GetAuthGameMode<AWorldGameMode>();
		if (gameMode && gameMode->GetComponentByClass<UWorldDirectionManager>()) {
			auto worldDirectionManager = gameMode->GetComponentByClass<UWorldDirectionManager>();
			worldDirectionManager->ClearDirections();
		}
	}
}