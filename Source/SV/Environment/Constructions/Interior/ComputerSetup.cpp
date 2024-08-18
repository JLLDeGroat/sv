// Fill out your copyright notice in the Description page of Project Settings.


#include "ComputerSetup.h"
#include "../../../Utilities/SvUtilities.h"

AComputerSetup::AComputerSetup(const FObjectInitializer& ObjectInitializer) : AEnvironmentActor(ObjectInitializer) {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootScene = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Scene"));
	RootComponent = RootScene;

	ScreenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Screen"));
	ScreenMesh->SetupAttachment(RootComponent);

	TowerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower"));
	TowerMesh->SetupAttachment(RootComponent);

	KeyboardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Keyboard"));
	KeyboardMesh->SetupAttachment(RootComponent);

	MouseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mouse"));
	MouseMesh->SetupAttachment(RootComponent);

	SetupEnvironmentMeshComponent(ScreenMesh);
	SetupEnvironmentMeshComponent(TowerMesh);
	SetupEnvironmentMeshComponent(KeyboardMesh);
	SetupEnvironmentMeshComponent(MouseMesh);
}

void AComputerSetup::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	FString screenRef = "/Script/Engine.StaticMesh'/Game/Environment/BuildingInterior/Pc/PC_PC.PC_PC'";
	FString towerRef = "/Script/Engine.StaticMesh'/Game/Environment/BuildingInterior/Pc/PC_DekstopTower.PC_DekstopTower'";
	FString keyboardRef = "/Script/Engine.StaticMesh'/Game/Environment/BuildingInterior/Pc/PC_Keyboard.PC_Keyboard'";
	FString mouseRef = "/Script/Engine.StaticMesh'/Game/Environment/BuildingInterior/Pc/ComputerMouse.ComputerMouse'";

	auto screen = USvUtilities::GetStaticMesh(screenRef);
	auto tower = USvUtilities::GetStaticMesh(towerRef);
	auto keyboard = USvUtilities::GetStaticMesh(keyboardRef);
	auto mouse = USvUtilities::GetStaticMesh(mouseRef);

	if (screen) {
		ScreenMesh->SetStaticMesh(screen);
		ScreenMesh->SetRelativeScale3D(FVector(.8));
	}

	if (tower) {
		TowerMesh->SetStaticMesh(tower);
		TowerMesh->SetRelativeLocation(FVector(17, 57, 17));
	}

	if (keyboard) {
		KeyboardMesh->SetStaticMesh(keyboard);
		KeyboardMesh->SetRelativeLocation(FVector(-52, -34, -22));
		KeyboardMesh->SetRelativeRotation(FRotator(0, 86, 0));
	}

	if (mouse) {
		MouseMesh->SetStaticMesh(mouse);
		MouseMesh->SetRelativeLocation(FVector(-28, 21, -24));
		MouseMesh->SetRelativeRotation(FRotator(180, 0, 0));
		MouseMesh->SetRelativeRotation(FRotator(0, 8, 0));
	}
}