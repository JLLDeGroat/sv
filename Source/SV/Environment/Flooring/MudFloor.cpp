// Fill out your copyright notice in the Description page of Project Settings.


#include "MudFloor.h"
#include "../../Utilities/SvUtilities.h"
#include "../Components/EnvironmentDetailsComponent.h"

AMudFloor::AMudFloor(const FObjectInitializer& ObjectInitializer)
	: AEnvironmentActor(ObjectInitializer) {


	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	RootComponent = FloorMesh;

	auto mesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Environment/Floor.Floor'");
	if (mesh) FloorMesh->SetStaticMesh(mesh);
	FloorMesh->SetWorldScale3D(FVector(.05f));

	FloorMesh->SetCollisionObjectType(USvUtilities::GetFloorTargetChannel());
	FloorMesh->SetCollisionResponseToChannel(USvUtilities::GetEnvironmentChannel(), ECR_Block);
	FloorMesh->SetCollisionResponseToChannel(USvUtilities::GetBulletCollisionChannel(), ECR_Overlap);
	FloorMesh->SetCollisionResponseToChannel(USvUtilities::GetBulletCollisionObjectChannel(), ECR_Overlap);
	FloorMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	FloorMesh->SetGenerateOverlapEvents(true);

	auto material = USvUtilities::GetMaterial("/Script/Engine.Material'/Game/Materials/Environment/Floor8K_M.Floor8K_M'");
	if (material) {
		FloorMesh->SetMaterial(0, material);
	}

	EnvironmentDetails = CreateDefaultSubobject<UEnvironmentDetailsComponent>(TEXT("Details"));
	EnvironmentDetails->SetThickness(100);
	EnvironmentDetails->SetAffectsFog(false);
}