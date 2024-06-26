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
	CurrentCameraState = ECameraState::CS_Control;
}


// Called when the game starts
void UPawnCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
	CameraComponent = GetOwner()->GetComponentByClass<UCameraComponent>();

	if (!CameraComponent)
		return UDebugMessages::LogError(this, "failed to get camera comonent");
}


// Called every frame
void UPawnCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentlyMoving) {
		auto currentLocation = GetOwner()->GetActorLocation();
		FVector newLocation = currentLocation;
		FRotator newRotation = CameraComponent->GetComponentRotation();
		FRotator requiredRotation = newRotation;

		if (CurrentCameraState == ECameraState::CS_CinematicShoot) {
			if (GetValidCinematicLocation(newLocation)) {
				requiredRotation = UGridUtilities::FindLookAtRotation(newLocation, CinematicActorTarget->GetActorLocation());
				newRotation = UKismetMathLibrary::RInterpTo_Constant(CameraComponent->GetComponentRotation(), requiredRotation, DeltaTime, 500);
			}
			else UDebugMessages::LogError(this, "failed to get valid cinematic location");
		}
		else {
			newLocation = UKismetMathLibrary::VInterpTo_Constant(currentLocation, CurrentMoveTo, DeltaTime, 800);

			requiredRotation = ShoudUseSetRotation() ?
				UGridUtilities::FindLookAtRotation(newLocation, CurrentRotateToLocation) :
				FRotator(-70, 0, 0);

			newRotation = UKismetMathLibrary::RInterpTo_Constant(CameraComponent->GetComponentRotation(), requiredRotation, DeltaTime, 500);
		}

		CameraComponent->SetWorldLocation(newLocation);
		CameraComponent->SetWorldRotation(newRotation);

		if (FVector::Dist(CameraComponent->GetComponentLocation(), CurrentMoveTo) < 5 &&
			CameraComponent->GetComponentRotation().Equals(requiredRotation)) {
			CurrentlyMoving = false;
			CurrentRotateToLocation = FVector::ZeroVector;
			SetComponentTickEnabled(false);
		
			if (CurrentCameraState == ECameraState::CS_ReTarget)
				CurrentCameraState = ECameraState::CS_Control;
		}
	}
}

void UPawnCameraComponent::UpdateCameraState(ECameraState cameraState, FVector moveToLocation, FVector lookAtLocation, bool overrideCameraMovements) {
	CurrentCameraState = cameraState;

	if (overrideCameraMovements) 
		return;

	if (cameraState == ECameraState::CS_Control) {
		CurrentMoveTo = ReturnLocation;
		CurrentRotateToLocation = lookAtLocation;

		auto playerController = GetWorld()->GetFirstPlayerController();
		playerController->bShowMouseCursor = true;
		playerController->SetInputMode(FInputModeGameOnly());

		CameraComponent->PostProcessSettings.DepthOfFieldMinFstop = DefaultDepthOfFieldFstopValue;
		CameraComponent->PostProcessSettings.DepthOfFieldFocalDistance = DefaultDepthOfFieldFocalDistance;
		CameraComponent->PostProcessSettings.DepthOfFieldSensorWidth = DefaultDepthOfFieldSensorWidth;
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

void UPawnCameraComponent::DoCinematicAttackCameraMovement(AActor* attacker, AActor* target) {
	CurrentCameraState = ECameraState::CS_CinematicShoot;

	CinematicActorAttacker = attacker;
	CinematicActorTarget = target;

	CurrentlyMoving = true;
	SetComponentTickEnabled(true);

	CameraComponent->PostProcessSettings.DepthOfFieldMinFstop = 4;
	CameraComponent->PostProcessSettings.DepthOfFieldSensorWidth = 36;
	CameraComponent->PostProcessSettings.DepthOfFieldFocalDistance = 1500;
	CameraComponent->FieldOfView = 90.0f;
}

ECameraState UPawnCameraComponent::GetCurrentCameraState() {
	return CurrentCameraState;
}

void UPawnCameraComponent::SetDefaultCameraOffset(FVector defaultValue) {
	DefaultCameraOffset = defaultValue;
}

bool UPawnCameraComponent::ShoudUseSetRotation() const {
	if (CurrentCameraState == ECameraState::CS_Control ||
		CurrentCameraState == ECameraState::CS_ReTarget || 
		CurrentCameraState == ECameraState::CS_ThrowTarget)
		return false;

	return true;
}

bool UPawnCameraComponent::GetValidCinematicLocation(FVector& location) {
	auto skeletalComponent = CinematicActorAttacker->GetComponentByClass<USkeletalMeshComponent>();
	if (!skeletalComponent) {
		return false;
	}

	location = skeletalComponent->GetSocketLocation(FName("RightShoulderSocket"));

	if (location == FVector::ZeroVector)
		return false;

	return true;
}