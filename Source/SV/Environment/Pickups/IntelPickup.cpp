// Fill out your copyright notice in the Description page of Project Settings.


#include "IntelPickup.h"
#include "Components/PickupDetailsComponent.h"
#include "Components/PickupMeshComponent.h"
#include "Components/PickupIndicatorLink.h"

// Sets default values
AIntelPickup::AIntelPickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PickupMeshComponent = CreateDefaultSubobject<UPickupMeshComponent>(TEXT("Pickup"));
	RootComponent = PickupMeshComponent;
	PickupMeshComponent->SetWorldScale3D(FVector(.5f));


	PickupDetailsComponent = CreateDefaultSubobject<UPickupDetailsComponent>(TEXT("PickupDetails"));
	PickupDetailsComponent->SetIsIntel();

	PickupIndicatorLink = CreateDefaultSubobject<UPickupIndicatorLink>(TEXT("IndicatorLink"));
}

void AIntelPickup::BeginPlay() {
	Super::BeginPlay();
	PickupMeshComponent->SetMesh("/Script/Engine.StaticMesh'/Game/Environment/Pickups/ClipBoard.ClipBoard'");
}

