// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSpawnerManager.h"
#include "../../Environment/Constructions/Wall.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
// Sets default values for this component's properties
ULevelSpawnerManager::ULevelSpawnerManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void ULevelSpawnerManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void ULevelSpawnerManager::SpawnMajorWallAt(FVector loc, FRotator rot) {
	UDebugMessages::LogDisplay(this, "spawning wall at " + loc.ToString());
	GetWorld()->SpawnActor<AWall>(loc, rot);

}