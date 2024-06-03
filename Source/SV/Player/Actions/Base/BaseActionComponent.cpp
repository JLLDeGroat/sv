// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseActionComponent.h"
#include "../../Managers/SelectionManager.h"
#include "../../Managers/ControlManager.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
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