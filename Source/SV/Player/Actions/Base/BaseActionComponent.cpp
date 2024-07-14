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
// Sets default values for this component's properties
UBaseActionComponent::UBaseActionComponent(const FObjectInitializer& ObjectInitializer) : UActorComponent(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

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

	UDebugMessages::LogWarning(this, "not in valid camera state to do action");
	return false;
}

void UBaseActionComponent::GetTargetLocation(FHitResult& hit, FVector& targetLocation, UCameraComponent* cameraComp) {
	auto cameraForwardVector = cameraComp->GetForwardVector();
	targetLocation = cameraComp->GetComponentLocation() + (5000 * cameraForwardVector);
	FCollisionObjectQueryParams collisionParams;
	collisionParams.AddObjectTypesToQuery(USvUtilities::GetBulletCollisionObjectChannel());
	collisionParams.AddObjectTypesToQuery(USvUtilities::GetEnvironmentChannel());
	collisionParams.AddObjectTypesToQuery(USvUtilities::GetFloorTargetChannel());
	GetWorld()->LineTraceSingleByObjectType(hit, cameraComp->GetComponentLocation(), targetLocation, collisionParams);

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