// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnCameraComponent.h"
#include "Camera/CameraComponent.h"
#include "../../Utilities/SvUtilities.h"
#include "../../Utilities/GridUtilities.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Characters/Components/HealthAndStatusWidgetComponent.h"

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


	auto currentLoc = CameraComponent->GetComponentLocation();
	auto forwardVec = CameraComponent->GetForwardVector();

	FHitResult hitResult;
	GetWorld()->LineTraceSingleByChannel(hitResult, currentLoc, (currentLoc + (forwardVec * 5000)), USvUtilities::GetFloorTargetChannel());
	if (hitResult.bBlockingHit || hitResult.GetActor()) {
		//DrawDebugLine(GetWorld(), currentLoc, hitResult.Location, FColor::Red, true, 100, 0, 4);
		DefaultCameraOffset = hitResult.Location - currentLoc;
		UDebugMessages::LogDisplay(this, DefaultCameraOffset.ToString());
		if (DefaultCameraOffset.X > 0)
			DefaultCameraOffset.X = DefaultCameraOffset.X * -1;

		if (DefaultCameraOffset.Y > 0)
			DefaultCameraOffset.Y = DefaultCameraOffset.Y * -1;

		DefaultCameraOffset.Z = currentLoc.Z;
	}
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

		if (CurrentCameraState == ECameraState::CS_CinematicShoot ||
			CurrentCameraState == ECameraState::CS_OverwatchCinematicShoot)
		{
			if (GetValidCinematicLocation(newLocation)) {
				requiredRotation = UGridUtilities::FindLookAtRotation(newLocation, CinematicActorTarget->GetActorLocation());
				newRotation = UKismetMathLibrary::RInterpTo_Constant(CameraComponent->GetComponentRotation(), requiredRotation, DeltaTime, 500);
			}
			else UDebugMessages::LogError(this, "failed to get valid cinematic location");
		}
		else {
			newLocation = UKismetMathLibrary::VInterpTo_Constant(currentLocation, CurrentMoveTo, DeltaTime, 1500);

			requiredRotation = ShoudUseSetRotation() ?
				UGridUtilities::FindLookAtRotation(newLocation, CurrentRotateToLocation) :
				FRotator(-70, 0, 0);

			newRotation = UKismetMathLibrary::RInterpTo_Constant(CameraComponent->GetComponentRotation(), requiredRotation, DeltaTime, 500);
		}

		//UDebugMessages::LogDisplay(this, "moving from: " + CameraComponent->GetComponentLocation().ToString() + " to: " +
		//	CurrentMoveTo.ToString() +
		//	" newloc: " + newLocation.ToString());

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

void UPawnCameraComponent::UpdateCameraState(ECameraState cameraState, FVector moveToLocation, FVector lookAtLocation, bool overrideCameraMovements, bool doNotSetReturn) {
	CurrentCameraState = cameraState;

	if (overrideCameraMovements)
		return;

	if (CurrentCameraState == ECameraState::CS_Control) {
		CurrentMoveTo = ReturnLocation;
		CurrentRotateToLocation = lookAtLocation;

		auto playerController = GetWorld()->GetFirstPlayerController();
		playerController->bShowMouseCursor = true;
		playerController->SetInputMode(FInputModeGameOnly());

		CameraComponent->PostProcessSettings.DepthOfFieldMinFstop = DefaultDepthOfFieldFstopValue;
		CameraComponent->PostProcessSettings.DepthOfFieldFocalDistance = DefaultDepthOfFieldFocalDistance;
		CameraComponent->PostProcessSettings.DepthOfFieldSensorWidth = DefaultDepthOfFieldSensorWidth;

		AttemptToAlterAttackerStatusWidgetVisibility(true);
	}
	else if (CurrentCameraState == ECameraState::CS_ReTarget) {
		CurrentMoveTo = moveToLocation + DefaultCameraOffset;
		AttemptToAlterAttackerStatusWidgetVisibility(true);
	}
	else {
		if (!doNotSetReturn) {
			ReturnLocation = CameraComponent->GetComponentLocation();
		}
		CurrentMoveTo = moveToLocation;
		CurrentRotateToLocation = lookAtLocation;
	}

	CurrentlyMoving = true;
	SetComponentTickEnabled(true);
}

void UPawnCameraComponent::DoCinematicAttackCameraMovement(AActor* attacker, AActor* target) {
	CurrentCameraState = ECameraState::CS_CinematicShoot;
	CinematicAttackCameraMovement(attacker, target);
}
void UPawnCameraComponent::DoOverwatchCinematicAttackCameraMovement(AActor* attacker, AActor* target) {
	CurrentCameraState = ECameraState::CS_OverwatchCinematicShoot;
	//ReturnLocation = CameraComponent->GetComponentLocation();
	CinematicAttackCameraMovement(attacker, target);
}
void UPawnCameraComponent::SetOverwatchReturnLocation(FVector loc) {
	ReturnLocation = loc;
}
void UPawnCameraComponent::CinematicAttackCameraMovement(AActor* attacker, AActor* target) {
	CinematicActorAttacker = attacker;
	CinematicActorTarget = target;

	AttemptToAlterAttackerStatusWidgetVisibility(false);

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

void UPawnCameraComponent::AttemptToAlterAttackerStatusWidgetVisibility(bool val) {
	if (CinematicActorAttacker) {
		auto healthAndStatusComponent = CinematicActorAttacker->GetComponentByClass<UHealthAndStatusWidgetComponent>();
		if (healthAndStatusComponent)
			healthAndStatusComponent->SetVisibility(val);
	}
}