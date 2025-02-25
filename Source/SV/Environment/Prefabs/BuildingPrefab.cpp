// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingPrefab.h"

// Sets default values
ABuildingPrefab::ABuildingPrefab(const FObjectInitializer &ObjectInitializer) : ABasePrefab(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABuildingPrefab::BeginPlay()
{
	Super::BeginPlay();
}

TArray<FVector> ABuildingPrefab::GetDoorLocations()
{
	return DoorLocations;
}
