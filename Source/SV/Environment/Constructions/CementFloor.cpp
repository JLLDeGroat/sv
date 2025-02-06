// Fill out your copyright notice in the Description page of Project Settings.


#include "CementFloor.h"
#include "../Components/EnvironmentDetailsComponent.h"
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

	FloorMeshComponent->SetCollisionObjectType(USvUtilities::GetFloorTargetChannel());
	FloorMeshComponent->SetCollisionResponseToChannel(USvUtilities::GetEnvironmentChannel(), ECR_Block);
	FloorMeshComponent->SetCollisionResponseToChannel(USvUtilities::GetBulletCollisionChannel(), ECR_Overlap);
	FloorMeshComponent->SetCollisionResponseToChannel(USvUtilities::GetBulletCollisionObjectChannel(), ECR_Overlap);
	FloorMeshComponent->SetCollisionResponseToChannel(USvUtilities::GetFloorTargetChannel(), ECR_Block);
	FloorMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	FloorMeshComponent->RecreatePhysicsState();

	DetailsComponent = CreateDefaultSubobject<UEnvironmentDetailsComponent>(TEXT("Details"));
	DetailsComponent->SetThickness(1);
	DetailsComponent->SetAffectsFog(false);
}
