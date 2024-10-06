// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldPlayerController.h"
#include "Actions/LeftClickAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/WorldDirectionOptionComponent.h"
#include "../Utilities/SvUtilities.h"

AWorldPlayerController::AWorldPlayerController() {
	bShowMouseCursor = true;
	bEnableClickEvents = true;

	PlayerInputMappingContext = LoadObject<UInputMappingContext>(this, TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Controls/InputMapping.InputMapping'"));
	ClickAction = LoadObject<UInputAction>(this, TEXT("/Script/EnhancedInput.InputAction'/Game/Controls/IA_Click.IA_Click'"));

	LeftClickActionComponent = CreateDefaultSubobject<ULeftClickAction>(TEXT("LeftClickAction"));

	DirectionOptionComponent = CreateDefaultSubobject<UWorldDirectionOptionComponent>(TEXT("WorldDirection"));
}

void AWorldPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AWorldPlayerController::BeginPlay() {
	Super::BeginPlay();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PlayerInputMappingContext, 0);
	}
}

void AWorldPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup touch input events
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Started, LeftClickActionComponent, &ULeftClickAction::DoWorldAction);
	}
	else
	{
		UDebugMessages::LogError(this, "Failed to ru, enhanced Input Component");
	}
}