// Fill out your copyright notice in the Description page of Project Settings.


#include "LeftClickAction.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Enums/ECharacterEnums.h"
#include "../Managers/SelectionManager.h"
#include "../Managers/ControlManager.h"
#include "../../Utilities/SvUtilities.h"
#include "../Components/PawnCameraComponent.h"
#include "Camera/CameraComponent.h"
#include "../../Characters/Components/EquipmentComponent.h"
#include "../../Characters/Components/TargetingComponent.h"
#include "../../Characters/Components/CharacterDetailsComponent.h"
#include "../../Characters/Components/AttackComponent.h"
#include "../../Characters/Components/ThrowableComponent.h"
#include "../../Interfaces/SvChar.h"
#include "../../Interfaces/Selectable.h"

// Sets default values for this component's properties
ULeftClickAction::ULeftClickAction(const FObjectInitializer& ObjectInitializer) : UBaseActionComponent(ObjectInitializer) 
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	ValidCameraStates.Emplace(ECameraState::CS_Control);
	ValidCameraStates.Emplace(ECameraState::CS_GunTarget);
	ValidCameraStates.Emplace(ECameraState::CS_ThrowTarget);
	// ...
}


// Called when the game starts
void ULeftClickAction::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void ULeftClickAction::DoAction() {
	auto controller = GetOwner<APlayerController>();
	auto pawn = controller->GetPawn();
	auto pawnCameraComponent = pawn->GetComponentByClass<UPawnCameraComponent>();
	auto pawnCamera = pawn->GetComponentByClass<UCameraComponent>();

	if (!IsInValidCameraState(pawnCameraComponent->GetCurrentCameraState()))
		return;

	FHitResult Hit;
	controller->GetHitResultUnderCursor(USvUtilities::GetClickableChannel(), false, Hit);

	if (!pawn || !pawnCameraComponent || !pawnCamera)
		return UDebugMessages::LogError(this, "could not get pawn, pawn camera or pawn camara component, stopping Action");

	if (pawnCameraComponent->GetCurrentCameraState() == ECameraState::CS_GunTarget) {
		auto cameraForwardVector = pawnCamera->GetForwardVector();
		auto targetLocation = pawnCamera->GetComponentLocation() + (5000 * cameraForwardVector);

		FHitResult TestHit;
		FCollisionObjectQueryParams collisionParams;
		collisionParams.AddObjectTypesToQuery(USvUtilities::GetBulletCollisionObjectChannel());
		collisionParams.AddObjectTypesToQuery(USvUtilities::GetEnvironmentChannel());
		GetWorld()->LineTraceSingleByObjectType(TestHit, pawnCamera->GetComponentLocation(), targetLocation, collisionParams);

		if (TestHit.GetActor())
			targetLocation = TestHit.Location;

		DrawDebugLine(GetWorld(), pawnCamera->GetComponentLocation(), targetLocation, FColor::Cyan, false, 60.0f, 0, 5);

		auto selected = SelectionManager->GetSelected();
		auto actor = selected->GetAsActor();
		auto targetingComponent = actor->GetComponentByClass<UTargetingComponent>();
		if (selected && targetingComponent) {
			auto currentTargetData = targetingComponent->GetCurrentMainTarget();

			auto equipmentComponent = actor->GetComponentByClass<UEquipmentComponent>();
			auto detailsComponent = actor->GetComponentByClass<UCharacterDetailsComponent>();

			if (!equipmentComponent || !detailsComponent)
				return UDebugMessages::LogError(this, "could not get equpiment or details component, will not begin try attack");

			detailsComponent->RemoveActionPoints(equipmentComponent->GetActionPointsNeededToUseEquipment());

			auto attackComponent = selected->GetAsActor()->GetComponentByClass<UAttackComponent>();
			attackComponent->TryAttackLocation(currentTargetData->GetShootLocation(), targetLocation);

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
	}
	else {
		if (Hit.GetActor() && SelectionManager->TrySetSelected(Hit.GetActor())) {
			SelectionManager->GetSelected()->TryVisualiseTargets();
		}
	}
}
