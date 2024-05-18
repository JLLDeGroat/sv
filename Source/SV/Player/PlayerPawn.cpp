// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/PawnCameraComponent.h"
#include "Components/CameraOverlapComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetRelativeRotation(FRotator(-70, 0, 0));
	Camera->SetRelativeLocation(FVector(0, 0, 1200));

	CameraComponent = CreateDefaultSubobject<UPawnCameraComponent>(TEXT("CameraMovement"));

	CameraOverlapComponent = CreateDefaultSubobject<UCameraOverlapComponent>(TEXT("OverlapComponent"));
	CameraOverlapComponent->SetupAttachment(Camera);
	CameraOverlapComponent->SetSphereRadius(30);
	CameraOverlapComponent->SetVisibility(true);
	CameraOverlapComponent->bHiddenInGame = false;

}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UPawnCameraComponent* APlayerPawn::GetPawnCameraComponent() {
	return CameraComponent;
}
UCameraComponent* APlayerPawn::GetCameraComponent() {
	return Camera;
}