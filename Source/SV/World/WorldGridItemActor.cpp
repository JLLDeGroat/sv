// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGridItemActor.h"
#include "../Utilities/SvUtilities.h"

// Sets default values
AWorldGridItemActor::AWorldGridItemActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;

	auto mesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/LevelPrototyping/Meshes/SM_Cube.SM_Cube'");
	if (mesh) {
		auto mat = USvUtilities::GetMaterial("/Script/Engine.Material'/Game/Materials/World/IsNormal_M.IsNormal_M'");
		MeshComponent->SetStaticMesh(mesh);
		MeshComponent->SetMaterial(0, mat);
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	bIsEnd = false;
	bIsStart = false;
	bIsOffshoot = false;
}

// Called when the game starts or when spawned
void AWorldGridItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWorldGridItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorldGridItemActor::SetIsStart() {
	auto mat = USvUtilities::GetMaterial("/Script/Engine.Material'/Game/Materials/World/IsStart_M.IsStart_M'");
	MeshComponent->SetMaterial(0, mat);
}
void AWorldGridItemActor::SetIsEnd() {
	auto mat = USvUtilities::GetMaterial("/Script/Engine.Material'/Game/Materials/World/IsEnd_M.IsEnd_M'");
	MeshComponent->SetMaterial(0, mat);
}
void AWorldGridItemActor::SetIsOffshoot() {
	auto mat = USvUtilities::GetMaterial("/Script/Engine.Material'/Game/Materials/World/IsOffshoot_M.IsOffshoot_M'");
	MeshComponent->SetMaterial(0, mat);
}
void AWorldGridItemActor::SetIsSpawn() {
	auto mat = USvUtilities::GetMaterial("/Script/Engine.Material'/Game/Materials/World/IsSpawn_M.IsSpawn_M'");
	MeshComponent->SetMaterial(0, mat);
}