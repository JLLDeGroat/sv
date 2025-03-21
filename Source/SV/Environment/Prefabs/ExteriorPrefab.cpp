// Fill out your copyright notice in the Description page of Project Settings.

#include "ExteriorPrefab.h"

// Sets default values
AExteriorPrefab::AExteriorPrefab(const FObjectInitializer &ObjectInitializer) : ABasePrefab(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AExteriorPrefab::BeginPlay()
{
	Super::BeginPlay();
}

TArray<FVector> AExteriorPrefab::GetStartLocations()
{
	return StartLocations;
}