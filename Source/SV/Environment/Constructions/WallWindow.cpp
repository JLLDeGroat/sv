// Fill out your copyright notice in the Description page of Project Settings.


#include "WallWindow.h"
#include "Components/StaticMeshComponent.h"
#include "../../Utilities/SvUtilities.h"
#include "../Components/VaultableComponent.h"
#include "../Components/EnvironmentDetailsComponent.h"

AWallWindow::AWallWindow(const FObjectInitializer& ObjectInitializer)
	: AEnvironmentActor(ObjectInitializer) {

	FenceMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetupEnvironmentMeshComponent(FenceMeshComponent);

	RootComponent = FenceMeshComponent;

	auto wallMesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Environment/WallWindow.WallWindow'");
	if (wallMesh) {
		FenceMeshComponent->SetStaticMesh(wallMesh);
	}
	VaultableComponent = CreateDefaultSubobject<UVaultableComponent>(TEXT("Vaultable"));
	VaultableComponent->SetupAttachment(RootComponent);
	VaultableComponent->SetRelativeLocation(FVector(0, -50, 50));
	VaultableComponent->SetBoxExtent(FVector(20, 50, 32));

	DetailsComponent = CreateDefaultSubobject<UEnvironmentDetailsComponent>(TEXT("Details"));
	DetailsComponent->SetThickness(1);
	DetailsComponent->SetAffectsFog(false);
}
