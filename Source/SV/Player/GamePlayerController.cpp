// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayerController.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputActionValue.h"
#include "../Utilities/GridUtilities.h"
#include "GridSelectionActor.h"
#include "../Interfaces/Selectable.h"
#include "../Interfaces/Movable.h"
#include "../Interfaces/SvChar.h"
#include "Managers/SelectionManager.h"
#include "../Characters/Components/GridMovementComponent.h"
#include "../Utilities/SvUtilities.h"
#include "PlayerPawn.h"
#include "../Characters/Components/TargetingComponent.h"
#include "../Characters/Components/AttackComponent.h"
#include "Components/PawnCameraComponent.h"
#include "Camera/CameraComponent.h"

#include "DrawDebugHelpers.h"

AGamePlayerController::AGamePlayerController() {
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;

	PlayerInputMappingContext = LoadObject<UInputMappingContext>(this, TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Controls/InputMapping.InputMapping'"));
	ClickAction = LoadObject<UInputAction>(this, TEXT("/Script/EnhancedInput.InputAction'/Game/Controls/IA_Click.IA_Click'"));
	RightClickAction = LoadObject<UInputAction>(this, TEXT("/Script/EnhancedInput.InputAction'/Game/Controls/IA_RightClick.IA_RightClick'"));
	MoveRightAction = LoadObject<UInputAction>(this, TEXT("/Script/EnhancedInput.InputAction'/Game/Controls/IA_MoveRight.IA_MoveRight'"));
	MoveUpAction = LoadObject<UInputAction>(this, TEXT("/Script/EnhancedInput.InputAction'/Game/Controls/IA_MoveUp.IA_MoveUp'"));
	BeginTargetAction = LoadObject<UInputAction>(this, TEXT("/Script/EnhancedInput.InputAction'/Game/Controls/IA_BeginTarget.IA_BeginTarget'"));
	CameraMoveAction = LoadObject<UInputAction>(this, TEXT("/Script/EnhancedInput.InputAction'/Game/Controls/IA_MouseMove.IA_MouseMove'"));

	SelectionManager = NewObject<USelectionManager>();
}

void AGamePlayerController::BeginPlay() {
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PlayerInputMappingContext, 0);
	}

	PlayerPawn = Cast<APlayerPawn>(GetPawn());

	if (MovementMultiplier < 10) {
		UDebugMessages::LogDisplay(this, "MovementMultiplier was too low, so set to 10 as default");
		MovementMultiplier = 10;
	}

	if (!GridSelection) {
		GridSelection = GetWorld()->SpawnActor<AGridSelectionActor>();
	}
}

void AGamePlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	FHitResult Hit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, Hit);

	if (Hit.GetActor()) {
		auto localised = UGridUtilities::GetNormalisedGridLocation(Hit.Location);

		if (SelectionManager->GetCurrentMousedLocation().X != localised.X ||
			SelectionManager->GetCurrentMousedLocation().Y != localised.Y)
		{
			GridSelection->SetActorLocation(localised);
			SelectionManager->SetCurrentMousedLocation(localised);

			auto selected = SelectionManager->GetSelected();
			TScriptInterface<IMovable> movable;
			if (selected && selected->TryGetAsMoveable(movable) &&
				selected->IsControlType(ECharacterControl::CC_Player))
			{
				auto movableComponent = movable->GetGridMovementComponent();
				auto startLocation = selected->GetSelectableGridLocation();

				auto gridSteps = movableComponent->FindRoute(startLocation, localised);
				SelectionManager->SetLocationPath(gridSteps);
				if (gridSteps.Num() > 1) {
					for (int i = 1; i < gridSteps.Num(); i++) {
						DrawDebugLine(GetWorld(), gridSteps[i - 1], gridSteps[i], FColor::Red, false, 1.0f, 0, 1.0f);
					}
				}
			}
		}
	}
}

void AGamePlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup touch input events
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Started, this, &AGamePlayerController::ClickAction_Started);
		//EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Triggered, this, &AGamePlayerController::OnSetCamLocTriggered_Touch);
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Completed, this, &AGamePlayerController::ClickAction_Released);
		//EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Canceled, this, &AGamePlayerController::OnSetCamLocCancelled_Touch);

		EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Started, this, &AGamePlayerController::RightClickAction_Started);
		//EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Triggered, this, &AGamePlayerController::OnSetCamLocTriggered_Touch);
		EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Completed, this, &AGamePlayerController::RightClickAction_Released);
		//EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Canceled, this, &AGamePlayerController::OnSetCamLocCancelled_Touch);

		EnhancedInputComponent->BindAction(MoveUpAction, ETriggerEvent::Triggered, this, &AGamePlayerController::MoveUp_Started);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &AGamePlayerController::MoveRight_Started);

		EnhancedInputComponent->BindAction(BeginTargetAction, ETriggerEvent::Started, this, &AGamePlayerController::BeginTarget_Started);

		EnhancedInputComponent->BindAction(CameraMoveAction, ETriggerEvent::Triggered, this, &AGamePlayerController::MouseMove);
	}
	else
	{
		UDebugMessages::LogError(this, "Failed to ru, enhanced Input Component");
	}

}

void AGamePlayerController::ClickAction_Started() {
	FHitResult Hit;
	GetHitResultUnderCursor(USvUtilities::GetClickableChannel(), false, Hit);

	if (PlayerPawn->GetPawnCameraComponent()->GetCurrentCameraState() == ECameraState::CS_GunTarget) {
		auto cameraForwardVector = PlayerPawn->GetCameraComponent()->GetForwardVector();
		auto distance = 5000; // TODO this should be decided by the equipment i think
		auto target = PlayerPawn->GetCameraComponent()->GetComponentLocation() + (distance * cameraForwardVector);

		DrawDebugLine(GetWorld(), PlayerPawn->GetCameraComponent()->GetComponentLocation(), target, FColor::Cyan, false, 60.0f, 0, 5);

		PlayerPawn->GetPawnCameraComponent()->UpdateCameraState(ECameraState::CS_Control);

		auto selected = SelectionManager->GetSelected();
		auto actor = selected->GetAsActor();
		auto targetingComponent = actor->GetComponentByClass<UTargetingComponent>();
		if (selected && targetingComponent) {
			auto currentTargetData = targetingComponent->GetCurrentTargetData();

			//assuming only one target data

			auto attackComponent = selected->GetAsActor()->GetComponentByClass<UAttackComponent>();
			attackComponent->TryAttackLocation(currentTargetData[0].GetShootLocation(), target);
		}
	}
	else {
		if (Hit.GetActor() && SelectionManager->TrySetSelected(Hit.GetActor())) {
			SelectionManager->GetSelected()->TryVisualiseTargets();
		}
	}
}
void AGamePlayerController::ClickAction_Released() {

}

void AGamePlayerController::RightClickAction_Started() {

	auto selected = SelectionManager->GetSelected();
	if (selected) {
		UDebugMessages::LogDisplay(this, "something selected");
		TScriptInterface<IMovable> movable;
		if (selected->TryGetAsMoveable(movable)) {
			auto selectedMouseLocation = SelectionManager->GetCurrentMousedLocation();
			auto currentActorGridLocation = selected->GetSelectableGridLocation();

			UDebugMessages::LogDisplay(this, "Moving from " + currentActorGridLocation.ToString() + " to: " + selectedMouseLocation.ToString());
			//testing grid system
			auto gridSteps = movable->GetGridMovementComponent()->FindRoute(currentActorGridLocation, selectedMouseLocation);
			//

			UDebugMessages::LogDisplay(this, "moving");
			movable->GetGridMovementComponent()->MoveAcrossGrid(SelectionManager->GetLocationPath());
		}
	}
}
void AGamePlayerController::RightClickAction_Released() {

}

void AGamePlayerController::BeginTarget_Started() {
	auto selected = SelectionManager->GetSelected();

	auto pawnCameraComponent = PlayerPawn->GetPawnCameraComponent();


	// reset values
	if (pawnCameraComponent->GetCurrentCameraState() == ECameraState::CS_GunTarget) {
		pawnCameraComponent->UpdateCameraState(ECameraState::CS_Control);
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());

		return;
	}

	if (selected) {
		auto actor = selected->GetAsActor();

		auto targetingComponent = actor->GetComponentByClass<UTargetingComponent>();

		if (targetingComponent) {
			auto currentTargetData = targetingComponent->GetCurrentTargetData();

			if (currentTargetData.Num() == 0) {
				UDebugMessages::LogError(this, "no targeting data");
				return;
			}

			//assuming only one target data

			PlayerPawn->GetPawnCameraComponent()->UpdateCameraState(ECameraState::CS_GunTarget, currentTargetData[0].GetShootLocation(),
				currentTargetData[0].GetCharacter()->GetSelectableGridLocation());

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}
	}
}

void AGamePlayerController::MoveUp_Started(const FInputActionValue& Value) {
	auto pawn = GetPawn();
	pawn->SetActorLocation(pawn->GetActorLocation() + FVector(Value.Get<float>() * MovementMultiplier, 0, 0));
}

void AGamePlayerController::MoveRight_Started(const FInputActionValue& Value) {
	auto pawn = GetPawn();
	pawn->SetActorLocation(pawn->GetActorLocation() + FVector(0, Value.Get<float>() * MovementMultiplier, 0));
}

void AGamePlayerController::MouseMove(const FInputActionValue& Value) {

	auto camera = PlayerPawn->GetCameraComponent();

	auto vectorValue = Value.Get<FVector2D>();

	auto additionalRotation = FRotator(vectorValue.Y, vectorValue.X, 0);
	auto current = camera->GetRelativeRotation();
	auto newRotation = additionalRotation + current;

	if (newRotation.Pitch > 60) newRotation.Pitch = 60;
	else if (newRotation.Pitch < -60) newRotation.Pitch = -60;

	camera->SetRelativeRotation(newRotation);
}