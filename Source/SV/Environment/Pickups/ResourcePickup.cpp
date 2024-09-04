// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourcePickup.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "../../Characters/Components/ActionsComponent.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Characters/Components/PickupResourceComponent.h"
#include "Components/PickupDetailsComponent.h"
#include "Components/PickupMeshComponent.h"
#include "Components/PickupCollisionComponent.h"
#include "../../Utilities/SvUtilities.h"

AResourcePickup::AResourcePickup(const FObjectInitializer& ObjectInitializer)
	: AEnvironmentActor(ObjectInitializer) {


	PickupMeshComponent = CreateDefaultSubobject<UPickupMeshComponent>(TEXT("Pickup"));
	RootComponent = PickupMeshComponent;
	PickupMeshComponent->SetWorldScale3D(FVector(.5f));


	BoxComponent = CreateDefaultSubobject<UPickupCollisionComponent>(TEXT("Box"));
	BoxComponent->SetupAttachment(RootComponent);

	PickupDetailsComponent = CreateDefaultSubobject<UPickupDetailsComponent>(TEXT("PickupDetails"));
}

void AResourcePickup::BeginPlay() {
	Super::BeginPlay();
	PickupMeshComponent->SetMesh("/Script/Engine.StaticMesh'/Game/Environment/Pickups/DroppedResource.DroppedResource'");
	PickupMeshComponent->SetSimulatePhysics(true);
}
