// Fill out your copyright notice in the Description page of Project Settings.


#include "Wall.h"
#include "Components/StaticMeshComponent.h"
#include "../Components/EnvironmentDetailsComponent.h"
#include "../../Utilities/SvUtilities.h"

AWall::AWall(const FObjectInitializer& ObjectInitializer)
	: AEnvironmentActor(ObjectInitializer) {

	WallMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetupEnvironmentMeshComponent(WallMeshComponent);

	RootComponent = WallMeshComponent;

	auto wallMesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Environment/Wall.Wall'");
	if (wallMesh) {
		WallMeshComponent->SetStaticMesh(wallMesh);
	}

	DetailsComponent = CreateDefaultSubobject<UEnvironmentDetailsComponent>(TEXT("Details"));
	DetailsComponent->SetThickness(1);
}