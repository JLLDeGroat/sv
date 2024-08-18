// Fill out your copyright notice in the Description page of Project Settings.


#include "CementFloor.h"

#include "Components/StaticMeshComponent.h"
#include "../../Utilities/SvUtilities.h"


ACementFloor::ACementFloor(const FObjectInitializer& ObjectInitializer)
	: AEnvironmentActor(ObjectInitializer) {

	FloorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetupEnvironmentMeshComponent(FloorMeshComponent);

	RootComponent = FloorMeshComponent;

	auto wallMesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Environment/CementFloor.CementFloor'");
	if (wallMesh) {
		FloorMeshComponent->SetStaticMesh(wallMesh);
	}
}
