// Fill out your copyright notice in the Description page of Project Settings.


#include "RockSection.h"
#include "../../Utilities/SvUtilities.h"

ARockSection::ARockSection(const FObjectInitializer& ObjectInitializer)
	: AEnvironmentActor(ObjectInitializer) {

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Rock1Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("R1Mesh"));
	Rock2Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("R2Mesh"));
	Rock3Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("R3Mesh"));
	Rock4Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("R4Mesh"));

	Rock1Mesh->SetupAttachment(RootComponent);
	Rock2Mesh->SetupAttachment(RootComponent);
	Rock3Mesh->SetupAttachment(RootComponent);
	Rock4Mesh->SetupAttachment(RootComponent);

	SetupEnvironmentMeshComponent(Rock1Mesh);
	SetupEnvironmentMeshComponent(Rock2Mesh);
	SetupEnvironmentMeshComponent(Rock3Mesh);
	SetupEnvironmentMeshComponent(Rock4Mesh);
}

void ARockSection::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	auto rock1MeshRef = "/Script/Engine.StaticMesh'/Game/Environment/RockSection.RockSection'";
	auto rock2MeshRef = "/Script/Engine.StaticMesh'/Game/Environment/RockSection2.RockSection2'";
	auto rock3MeshRef = "/Script/Engine.StaticMesh'/Game/Environment/RockSection3.RockSection3'";
	auto rock4MeshRef = "/Script/Engine.StaticMesh'/Game/Environment/RockSection4.RockSection4'";

	auto rock1Mesh = USvUtilities::GetStaticMesh(rock1MeshRef);
	auto rock2Mesh = USvUtilities::GetStaticMesh(rock2MeshRef);
	auto rock3Mesh = USvUtilities::GetStaticMesh(rock3MeshRef);
	auto rock4Mesh = USvUtilities::GetStaticMesh(rock4MeshRef);

	SetActorScale3D(FVector(1, 1, 1));

	if (rock1Mesh) {
		Rock1Mesh->SetStaticMesh(rock1Mesh);
	}
	if (rock2Mesh) {
		Rock2Mesh->SetStaticMesh(rock2Mesh);
	}
	if (rock3Mesh) {
		Rock3Mesh->SetStaticMesh(rock3Mesh);
	}
	if (rock4Mesh) {
		Rock4Mesh->SetStaticMesh(rock4Mesh);
	}

	DictatePosition();
	DictateRotation();
}

void ARockSection::DictatePosition() {
	TArray<UStaticMeshComponent*> meshes;
	meshes.Emplace(Rock1Mesh);
	meshes.Emplace(Rock2Mesh);
	meshes.Emplace(Rock3Mesh);
	meshes.Emplace(Rock4Mesh);

	int currentZ = 0;
	while (meshes.Num() > 0) {
		for (int i = 0; i < meshes.Num(); i++) {
			if (FMath::RandRange(1, 101) > 60) {
				meshes[i]->SetRelativeLocation(FVector(0, 0, currentZ));
				currentZ += 50;
				meshes.RemoveAt(i);
				break;
			}
		}
	}
}
void ARockSection::DictateRotation() {
	auto randomRot1 = FRotator(0, FMath::RandRange(0, 360), 0);
	auto randomRot2 = FRotator(0, FMath::RandRange(0, 360), 0);
	auto randomRot3 = FRotator(0, FMath::RandRange(0, 360), 0);
	auto randomRot4 = FRotator(0, FMath::RandRange(0, 360), 0);

	Rock1Mesh->SetRelativeRotation(randomRot1);
	Rock2Mesh->SetRelativeRotation(randomRot2);
	Rock3Mesh->SetRelativeRotation(randomRot3);
	Rock4Mesh->SetRelativeRotation(randomRot4);
}