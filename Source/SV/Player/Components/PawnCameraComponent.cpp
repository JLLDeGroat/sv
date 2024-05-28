// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnCameraComponent.h"
#include "Camera/CameraComponent.h"
#include "../../Utilities/GridUtilities.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

// Sets default values for this component's properties
UPawnCameraComponent::UPawnCameraComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPawnCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
	CameraComponent = GetOwner()->GetComponentByClass<UCameraComponent>();
}


// Called every frame
void UPawnCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentlyMoving) {
		auto currentLocation = GetOwner()->GetActorLocation();

		//if (UseOffsetMovement) {
		//	currentLocation += DefaultCameraOffset;
		//}

		auto newLocation = UKismetMathLibrary::VInterpTo_Constant(currentLocation, CurrentMoveTo, DeltaTime, 800);
		
		auto lookAtRot = ShoudUseSetRotation() ?
			UGridUtilities::FindLookAtRotation(newLocation, CurrentRotateToLocation) :
			FRotator(-70, 0, 0);

		auto newRotation = UKismetMathLibrary::RInterpTo_Constant(CameraComponent->GetComponentRotation(), lookAtRot, DeltaTime, 500);

		CameraComponent->SetWorldLocation(newLocation);
		CameraComponent->SetWorldRotation(newRotation);

		if (FVector::Dist(CameraComponent->GetComponentLocation(), CurrentMoveTo) < 5 &&
			CameraComponent->GetComponentRotation().Equals(lookAtRot)) {
			CurrentlyMoving = false;
			CurrentRotateToLocation = FVector::ZeroVector;
			SetComponentTickEnabled(false);
		}
	}
}

void UPawnCameraComponent::UpdateCameraState(ECameraState cameraState, FVector moveToLocation, FVector lookAtLocation) {
	CurrentCameraState = cameraState;
	if (cameraState == ECameraState::CS_Control) {
		CurrentMoveTo = ReturnLocation;
		CurrentRotateToLocation = lookAtLocation;

		auto playerController = GetWorld()->GetFirstPlayerController();
		playerController->bShowMouseCursor = true;
		playerController->SetInputMode(FInputModeGameOnly());
	}
	else if (cameraState == ECameraState::CS_ReTarget) {
		CurrentMoveTo = moveToLocation += DefaultCameraOffset;
	}
	else {
		ReturnLocation = CameraComponent->GetComponentLocation();

		CurrentMoveTo = moveToLocation;
		CurrentRotateToLocation = lookAtLocation;
	}

	CurrentlyMoving = true;
	SetComponentTickEnabled(true);
}

ECameraState UPawnCameraComponent::GetCurrentCameraState() {
	return CurrentCameraState;
}

void UPawnCameraComponent::SetDefaultCameraOffset(FVector defaultValue) {
	DefaultCameraOffset = defaultValue;
}

bool UPawnCameraComponent::ShoudUseSetRotation() const {
	if (CurrentCameraState == ECameraState::CS_Control || 
		CurrentCameraState == ECameraState::CS_ReTarget) 
		return false;
	
	return true;
}