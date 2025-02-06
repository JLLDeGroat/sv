// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseActionComponent.h"
#include "../../Managers/SelectionManager.h"
#include "../../Managers/ControlManager.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Camera/CameraComponent.h"
#include "../../../Utilities/SvUtilities.h"
#include "../TargetAction.h"
#include "../ActivateToggleAction.h"
#include "../GrenadeActionComponent.h"
#include "../LeftClickAction.h"
#include "../RightClickAction.h"
#include "../../../Delegates/GameplayDelegates.h"
#include "../../../Interfaces/Selectable.h"
#include "../../Components/PawnCameraComponent.h"
// Sets default values for this component's properties
UBaseActionComponent::UBaseActionComponent(const FObjectInitializer& ObjectInitializer) : UActorComponent(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	ValidControlLimits.Emplace(EControlLimit::CL_NONE);
	// ...
}


// Called when the game starts
void UBaseActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	SelectionManager = GetOwner()->GetComponentByClass<USelectionManager>();
	ControlManager = GetOwner()->GetComponentByClass<UControlManager>();
}

AGamePlayerController* UBaseActionComponent::GetOwningController() {
	if (!PlayerController) {
		PlayerController = GetOwner<AGamePlayerController>();
	}
	return PlayerController;
}

bool UBaseActionComponent::IsInValidCameraState(ECameraState currentCameraState) {
	for (int i = 0; i < ValidCameraStates.Num(); i++) {
		if (ValidCameraStates[i] == currentCameraState) return true;
	}

	UDebugMessages::LogWarning(this, "not in valid camera state to do action, current state " + ECharacterEnums::GetCameraStateAsString(currentCameraState));
	return false;
}

bool UBaseActionComponent::IsInValidCameraState() {
	auto pawn = GetOwningController()->GetPawn();
	auto pawnCamera = pawn->GetComponentByClass<UPawnCameraComponent>();
	return IsInValidCameraState(pawnCamera->GetCurrentCameraState());
}

void UBaseActionComponent::GetTargetLocation(FHitResult& hit, FVector& targetLocation, UCameraComponent* cameraComp) {
	if (!cameraComp)
		return UDebugMessages::LogError(this, "camera comp is null");

	auto cameraForwardVector = cameraComp->GetForwardVector();
	targetLocation = cameraComp->GetComponentLocation() + (5000 * cameraForwardVector);
	FCollisionObjectQueryParams collisionParams;
	collisionParams.AddObjectTypesToQuery(USvUtilities::GetBulletCollisionObjectChannel());
	collisionParams.AddObjectTypesToQuery(USvUtilities::GetEnvironmentChannel());
	collisionParams.AddObjectTypesToQuery(USvUtilities::GetFloorTargetChannel());

	auto selectionComponent = GetOwner()->GetComponentByClass<USelectionManager>();
	if (!SelectionManager->GetSelected()) {
		return UDebugMessages::LogError(this, "failed to get selected actor");
	}

	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(SelectionManager->GetSelected()->GetAsActor());

	GetWorld()->LineTraceSingleByObjectType(hit, cameraComp->GetComponentLocation(), targetLocation, collisionParams, queryParams);

	if (hit.GetActor())
		targetLocation = hit.Location;
}

void UBaseActionComponent::ResetActionEffects() {
	auto controller = GetOwningController();
	auto targetAction = controller->GetComponentByClass<UTargetAction>();
	if (targetAction) {
		targetAction->ResetTargetingActor();
	}
}

void UBaseActionComponent::UpdateControlLimit(EControlLimit controlLimit) {
	auto gamePlay = UGameplayDelegates::GetInstance();
	if (!gamePlay)
		return UDebugMessages::LogError(this, "failed to get gameplay delegates");

	gamePlay->_ChangeControlLimits.Broadcast(controlLimit);
}

bool UBaseActionComponent::IsWithinValidControlLimiter() {
	auto controller = GetOwningController();
	auto controlAction = controller->GetComponentByClass<UControlManager>();
	if (controlAction) {
		auto currentLimit = controlAction->GetControlLimit();

		for (int i = 0; i < ValidControlLimits.Num(); i++)
			if (currentLimit == ValidControlLimits[i])
				return true;
	}

	UDebugMessages::LogWarning(this, "is not within valid control limiter, cannot do action");
	return false;
}