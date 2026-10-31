// Fill out your copyright notice in the Description page of Project Settings.

#include "ClothingHelmet.h"
#include "Components/SceneComponent.h"

// Sets default values
AClothingHelmet::AClothingHelmet(const FObjectInitializer &ObjectInitializer) : AClothing(ObjectInitializer)
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AClothingHelmet::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AClothingHelmet::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
