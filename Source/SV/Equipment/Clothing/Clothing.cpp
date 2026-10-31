// Fill out your copyright notice in the Description page of Project Settings.

#include "Clothing.h"
#include "Components/ClothingStatsComponent.h"
#include "Components/SceneComponent.h"
#include "Components/ClothingAttachComponent.h"
#include "Components/ClothingDamageReceiveComponent.h"

// Sets default values
AClothing::AClothing(const FObjectInitializer &ObjectInitializer) : AActor(ObjectInitializer)
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    StatsComponent = CreateDefaultSubobject<UClothingStatsComponent>(TEXT("ClothingStatsComponent"));
    AttachComponent = CreateDefaultSubobject<UClothingAttachComponent>(TEXT("ClothingAttachComponent"));
    DamageReceiveComponent = CreateDefaultSubobject<UClothingDamageReceiveComponent>(TEXT("DamageReceiveComponent"));

    ClothingType = EClothingType::INVALID;
    ClothingGrade = EClothingGrade::INVALID;
}

// Called when the game starts or when spawned
void AClothing::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AClothing::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AClothing::SetupClothingMeshComponent(UStaticMeshComponent *component)
{
    component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}