// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerSpawnIndicator.h"
#include "Components/IndicatorActivatorComponent.h"

// Sets default values
APlayerSpawnIndicator::APlayerSpawnIndicator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ActivatorIndicator->SetIndicatorType(EIndicatorType::IT_Spawn);
}
// Called when the game starts or when spawned
void APlayerSpawnIndicator::BeginPlay()
{
	Super::BeginPlay();
	ActivatorIndicator->ActivateIndicator(nullptr);
}

// Called every frame
void APlayerSpawnIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
