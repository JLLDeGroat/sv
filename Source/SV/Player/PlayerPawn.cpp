// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/PawnCameraComponent.h"
#include "Components/CameraOverlapComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Components/PostProcessComponent.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetRelativeRotation(FRotator(-70, 0, 0));

	auto defaultLocation = FVector(200, 0, 600);
	Camera->SetRelativeLocation(defaultLocation);

	CameraComponent = CreateDefaultSubobject<UPawnCameraComponent>(TEXT("CameraMovement"));
	CameraComponent->SetDefaultCameraOffset(defaultLocation);

	CameraOverlapComponent = CreateDefaultSubobject<UCameraOverlapComponent>(TEXT("OverlapComponent"));
	CameraOverlapComponent->SetupAttachment(Camera);
	CameraOverlapComponent->SetSphereRadius(30);

	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess"));

	PostProcessComponent->Settings.bOverride_DepthOfFieldMinFstop = true;
	PostProcessComponent->Settings.bOverride_DepthOfFieldSensorWidth = true;
	PostProcessComponent->Settings.bOverride_DepthOfFieldFocalDistance = true;

	/*DefaultDepthOfFieldFstopValue = Camera->PostProcessSettings.DepthOfFieldMinFstop;
	DefaultDepthOfFieldFocalDistance = Camera->PostProcessSettings.DepthOfFieldFocalDistance;
	DefaultDepthOfFieldSensorWidth = Camera->PostProcessSettings.DepthOfFieldSensorWidth;*/

	PostProcessComponent->Settings.DepthOfFieldMinFstop = .4f;
	PostProcessComponent->Settings.DepthOfFieldSensorWidth = 144;
	PostProcessComponent->Settings.DepthOfFieldFocalDistance = 600;
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);

	PostProcessComponent->Activate();
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