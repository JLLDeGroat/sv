// Fill out your copyright notice in the Description page of Project Settings.


#include "LongTable.h"

#include "../../../Utilities/SvUtilities.h"
// Sets default values
ALongTable::ALongTable(const FObjectInitializer& ObjectInitializer)
	: AEnvironmentActor(ObjectInitializer) {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootTable"));
	RootComponent = MeshComponent;
	SetupEnvironmentMeshComponent(MeshComponent);

	Wheel1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel1"));
	Wheel2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel2"));
	Wheel3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel3"));
	Wheel4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel4"));

	Wheel1->SetupAttachment(RootComponent);
	Wheel2->SetupAttachment(RootComponent);
	Wheel3->SetupAttachment(RootComponent);
	Wheel4->SetupAttachment(RootComponent);

	Wheel1->SetWorldScale3D(FVector(.45f));
	Wheel2->SetWorldScale3D(FVector(.45f));
	Wheel3->SetWorldScale3D(FVector(.45f));
	Wheel4->SetWorldScale3D(FVector(.45f));
}

void ALongTable::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	FString tableMeshRef = "/Script/Engine.StaticMesh'/Game/Environment/BuildingInterior/LongTable.LongTable'";
	FString tableWheelRef = "/Script/Engine.StaticMesh'/Game/Environment/BuildingInterior/RollingTable_TableWheel.RollingTable_TableWheel'";

	auto tableMesh = USvUtilities::GetStaticMesh(tableMeshRef);
	auto tableWheel = USvUtilities::GetStaticMesh(tableWheelRef);

	if (tableMesh) {
		MeshComponent->SetStaticMesh(tableMesh);
	}

	if (tableWheel) {
		Wheel1->SetStaticMesh(tableWheel);
		Wheel2->SetStaticMesh(tableWheel);
		Wheel3->SetStaticMesh(tableWheel);
		Wheel4->SetStaticMesh(tableWheel);
	}

	Wheel1->SetRelativeLocation(FVector(289, 86, 0));
	Wheel2->SetRelativeLocation(FVector(289, 11, 0));
	Wheel3->SetRelativeLocation(FVector(12, 86, 0));
	Wheel4->SetRelativeLocation(FVector(12, 11, 0));

	Wheel1->SetRelativeRotation(FRotator(0, FMath::RandRange(0, 360), 0));
	Wheel2->SetRelativeRotation(FRotator(0, FMath::RandRange(0, 360), 0));
	Wheel3->SetRelativeRotation(FRotator(0, FMath::RandRange(0, 360), 0));
	Wheel4->SetRelativeRotation(FRotator(0, FMath::RandRange(0, 360), 0));
}