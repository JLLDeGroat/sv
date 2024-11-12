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

	/*auto wallMesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Environment/Wall.Wall'");
	if (wallMesh) {
		WallMeshComponent->SetStaticMesh(wallMesh);
	}*/

	auto wallMeshV2 = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Environment/Wallv2.Wallv2'");
	if (wallMeshV2)
		WallMeshComponent->SetStaticMesh(wallMeshV2);

	auto iteration = FMath::RandRange(1, 4);
	FString sanitized = FString::SanitizeFloat(iteration, 0);
	auto matRef = "/Script/Engine.Material'/Game/Materials/Environment/CementWall/CementWall" + sanitized + "_M.CementWall" + sanitized + "_M'";


	DetailsComponent = CreateDefaultSubobject<UEnvironmentDetailsComponent>(TEXT("Details"));
	DetailsComponent->SetThickness(1);
}