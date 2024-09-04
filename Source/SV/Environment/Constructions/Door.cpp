// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "../Components/ToggleMovementComponent.h"
#include "../Components/ToggleRadiusComponent.h"
#include "../../Utilities/SvUtilities.h"

ADoor::ADoor(const FObjectInitializer& ObjectInitializer)
	: AEnvironmentActor(ObjectInitializer)
{
	DoorFrameComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	RootComponent = DoorFrameComponent;

	DoorComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	DoorComponent->SetupAttachment(RootComponent);

	auto doorFrame = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Environment/Door_DoorFrame.Door_DoorFrame'");
	auto door = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Environment/Door_Door.Door_Door'");

	if (doorFrame && door) {
		DoorComponent->SetStaticMesh(door);
		DoorFrameComponent->SetStaticMesh(doorFrame);
	}

	ToggleMovement = CreateDefaultSubobject<UToggleMovementComponent>(TEXT("Toggle"));

	ToggleRadius = CreateDefaultSubobject<UToggleRadiusComponent>(TEXT("Radius"));
	ToggleRadius->SetupAttachment(RootComponent);

	ToggleRadius->SetBoxExtent(FVector(100, 150, 100));
	ToggleRadius->SetRelativeLocation(FVector(0, -50, 100));

	SetupEnvironmentMeshComponent(DoorComponent);
	SetupEnvironmentMeshComponent(DoorFrameComponent);
}

void ADoor::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	ToggleMovement->Setup(DoorComponent, FVector::ZeroVector, FVector(0, 0, -200));
}