// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePlayerController.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputActionValue.h"
#include "../Utilities/GridUtilities.h"
#include "Utility/GridSelectionActor.h"
#include "Utility/GrenadeIndicatorActor.h"
#include "../Interfaces/Selectable.h"
#include "../Interfaces/Movable.h"
#include "../Interfaces/SvChar.h"
#include "../Delegates/GameplayDelegates.h"
#include "Managers/SelectionManager.h"
#include "Managers/ControlManager.h"
#include "../Characters/Components/GridMovementComponent.h"
#include "../Utilities/SvUtilities.h"
#include "PlayerPawn.h"
#include "../Characters/Components/TargetingComponent.h"
#include "../Characters/Components/AttackComponent.h"
#include "../Characters/Components/CharacterDetailsComponent.h"
#include "../Characters/Components/EquipmentComponent.h"
#include "../Characters/Components/ThrowableComponent.h"
#include "Components/PawnCameraComponent.h"
#include "Camera/CameraComponent.h"
#include "Actions/Base/ActionManager.h"
#include "Actions/GrenadeActionComponent.h"
#include "Actions/LeftClickAction.h"
#include "Actions/RightClickAction.h"
#include "Actions/TargetAction.h"
#include "Actions/ActivateToggleAction.h"
#include "Actions/SleepAction.h"
#include "Actions/ReloadAction.h"
#include "Actions/CycleTargetAction.h"
#include "Actions/OverwatchAction.h"
#include "Components/CameraShakeComponent.h"
#include "Components/CapsuleComponent.h"
#include "../GameModes/GameplayMode.h"

#include "DrawDebugHelpers.h"

AGamePlayerController::AGamePlayerController()
{
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
	GrenadeAction = LoadObject<UInputAction>(this, TEXT("/Script/EnhancedInput.InputAction'/Game/Controls/IA_UseGrenade.IA_UseGrenade'"));
	ActivateToggleAction = LoadObject<UInputAction>(this, TEXT("/Script/EnhancedInput.InputAction'/Game/Controls/IA_ActivateToggle.IA_ActivateToggle'"));
	SleepAction = LoadObject<UInputAction>(this, TEXT("/Script/EnhancedInput.InputAction'/Game/Controls/IA_SleepAction.IA_SleepAction'"));
	ReloadAction = LoadObject<UInputAction>(this, TEXT("/Script/EnhancedInput.InputAction'/Game/Controls/IA_Reload.IA_Reload'"));
	CycleAction = LoadObject<UInputAction>(this, TEXT("/Script/EnhancedInput.InputAction'/Game/Controls/IA_CycleTarget.IA_CycleTarget'"));
	OverwatchAction = LoadObject<UInputAction>(this, TEXT("/Script/EnhancedInput.InputAction'/Game/Controls/IA_Overwatch.IA_Overwatch'"));
	ExtractionAction = LoadObject<UInputAction>(this, TEXT("/Script/EnhancedInput.InputAction'/Game/Controls/IA_Extraction.IA_Extraction'"));

	SelectionManager = CreateDefaultSubobject<USelectionManager>(TEXT("Selection"));
	ControlManager = CreateDefaultSubobject<UControlManager>(TEXT("Control"));
	ActionManager = CreateDefaultSubobject<UActionManager>(TEXT("ActionManager"));

	CameraShakeComponent = CreateDefaultSubobject<UCameraShakeComponent>(TEXT("ShakeComp"));
}

void AGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PlayerInputMappingContext, 0);
	}

	PlayerPawn = Cast<APlayerPawn>(GetPawn());

	if (MovementMultiplier < 10)
	{
		UDebugMessages::LogDisplay(this, "MovementMultiplier was too low, so set to 10 as default");
		MovementMultiplier = 10;
	}

	if (!GridSelection)
	{
		GridSelection = GetWorld()->SpawnActor<AGridSelectionActor>();
		ControlManager->SetGridSelectionActor(GridSelection);
	}
	if (!GrenadeIndicator)
	{
		GrenadeIndicator = GetWorld()->SpawnActor<AGrenadeIndicatorActor>();
		ControlManager->SetGrenadeIndicatorActor(GrenadeIndicator);
		ActionManager->GetGrenadeAction()->SetGrenadeIndicatorActor(GrenadeIndicator);
		GrenadeIndicator->SetActorLocation(FVector(0, 0, -1000));
	}
}

void AGamePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult Hit;
	FVector WorldLocation, WorldDirection;
	DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

	FCollisionQueryParams QueryParams;
	QueryParams.bReturnPhysicalMaterial = false;
	FCollisionObjectQueryParams objectQueryParams;
	objectQueryParams.AddObjectTypesToQuery(USvUtilities::GetFloorTargetChannel());

	if (GetWorld()->LineTraceSingleByObjectType(Hit, WorldLocation, WorldLocation + (WorldDirection * 5000.f), objectQueryParams, QueryParams))
	{
		if (Hit.GetActor())
		{
			if (ControlManager->GetCanMouseDesignateExplosionRadiusActor())
				ControlManager->TickShowGrenadeIndicator(Hit.Location);

			if (ControlManager->GetCanMouseDesignateSelectionDecal())
			{
				auto localised = UGridUtilities::GetNormalisedGridLocation(Hit.Location);

				if (SelectionManager->GetCurrentMousedLocation().X != localised.X ||
					SelectionManager->GetCurrentMousedLocation().Y != localised.Y)
				{
					GridSelection->SetActorLocation(localised);
					ControlManager->TickFindMovementPath(localised);
				}
			}
		}
	}
}

USelectionManager *AGamePlayerController::GetSelectionManager()
{
	return SelectionManager;
}
UControlManager *AGamePlayerController::GetControlManager()
{
	return ControlManager;
}

void AGamePlayerController::SetMouseAsUi()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	SetInputMode(FInputModeGameAndUI());
	ControlManager->SetCanMouseDesignateSelectionDecal(true);
}
void AGamePlayerController::SetMouseAsGame()
{
	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableTouchEvents = false;
	SetInputMode(FInputModeGameOnly());
	ControlManager->SetCanMouseDesignateSelectionDecal(false);
}

void AGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup touch input events
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Started, ActionManager->GetLeftClickAction(), &ULeftClickAction::DoAction);
		EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Started, ActionManager->GetGetRightClickAction(), &URightClickAction::DoAction);
		EnhancedInputComponent->BindAction(MoveUpAction, ETriggerEvent::Triggered, this, &AGamePlayerController::MoveUp_Started);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &AGamePlayerController::MoveRight_Started);
		EnhancedInputComponent->BindAction(CameraMoveAction, ETriggerEvent::Triggered, this, &AGamePlayerController::MouseMove);
		EnhancedInputComponent->BindAction(BeginTargetAction, ETriggerEvent::Started, ActionManager->GetTargetAction(), &UTargetAction::DoAction);
		EnhancedInputComponent->BindAction(GrenadeAction, ETriggerEvent::Started, ActionManager->GetGrenadeAction(), &UGrenadeActionComponent::DoAction);
		EnhancedInputComponent->BindAction(ActivateToggleAction, ETriggerEvent::Started, ActionManager->GetActivateToggleAction(), &UActivateToggleAction::DoAction);
		EnhancedInputComponent->BindAction(SleepAction, ETriggerEvent::Started, ActionManager->GetSleepAction(), &USleepAction::DoAction);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, ActionManager->GetReloadAction(), &UReloadAction::DoAction);
		EnhancedInputComponent->BindAction(CycleAction, ETriggerEvent::Started, ActionManager->GetCycleTargetAction(), &UCycleTargetAction::DoAction);
		EnhancedInputComponent->BindAction(OverwatchAction, ETriggerEvent::Started, ActionManager->GetOverwatchAction(), &UOverwatchAction::DoAction);
	}
	else
	{
		UDebugMessages::LogError(this, "Failed to ru, enhanced Input Component");
	}
}

void AGamePlayerController::MoveUp_Started(const FInputActionValue &Value)
{
	if (PlayerPawn->GetPawnCameraComponent()->GetCurrentCameraState() != ECameraState::CS_GunTarget)
		PlayerPawn->SetActorLocation(PlayerPawn->GetActorLocation() + FVector(Value.Get<float>() * MovementMultiplier, 0, 0));
}

void AGamePlayerController::MoveRight_Started(const FInputActionValue &Value)
{
	if (PlayerPawn->GetPawnCameraComponent()->GetCurrentCameraState() != ECameraState::CS_GunTarget)
		PlayerPawn->SetActorLocation(PlayerPawn->GetActorLocation() + FVector(0, Value.Get<float>() * MovementMultiplier, 0));
}

void AGamePlayerController::MouseMove(const FInputActionValue &Value)
{
	if (PlayerPawn->GetPawnCameraComponent()->GetCurrentCameraState() == ECameraState::CS_GunTarget)
	{
		auto camera = PlayerPawn->GetCameraComponent();

		auto vectorValue = Value.Get<FVector2D>();

		auto additionalRotation = FRotator(vectorValue.Y, vectorValue.X, 0);
		auto current = camera->GetRelativeRotation();
		auto newRotation = additionalRotation + current;

		if (newRotation.Pitch > 60)
			newRotation.Pitch = 60;
		else if (newRotation.Pitch < -60)
			newRotation.Pitch = -60;

		camera->SetRelativeRotation(newRotation);
	}
}