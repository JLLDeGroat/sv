// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

// Sets default values
AEquipment::AEquipment(const FObjectInitializer& ObjectInitializer) : AActor(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEquipment::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEquipment::SetupAttachVector() {
	UDebugMessages::LogError(this, "This method should not be hit, equipment does not setup attach vectors");
}