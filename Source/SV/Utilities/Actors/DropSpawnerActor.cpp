// Fill out your copyright notice in the Description page of Project Settings.


#include "DropSpawnerActor.h"
#include "../SvUtilities.h"
#include "../../Environment/Pickups/ResourcePickup.h"
#include "../../Environment/Pickups/Components/PickupDetailsComponent.h"
#include "../../Environment/Pickups/IntelPickup.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
// Sets default values
ADropSpawnerActor::ADropSpawnerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootMesh->SetCanEverAffectNavigation(false);
	RootComponent = RootMesh;

	auto mesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'");
	if (mesh)
		RootMesh->SetStaticMesh(mesh);
}


void ADropSpawnerActor::BeginPlay() {
	Super::BeginPlay();

	if (ResType != EResourceType::INVALID) {
		auto res = GetWorld()->SpawnActor<AResourcePickup>(GetActorLocation(), FRotator::ZeroRotator, FActorSpawnParameters());

		if (!res)
			UDebugMessages::LogError(this, "failed to spawn resource drop actor");
		else {
			auto details = res->GetComponentByClass<UPickupDetailsComponent>();
			if (details) {
				details->SetResourceType(ResType);
				details->SetAmount(Amount);
			}
		}
	}
	else {
		auto res = GetWorld()->SpawnActor<AIntelPickup>(GetActorLocation(), FRotator::ZeroRotator, FActorSpawnParameters());
	}

	Destroy();
}