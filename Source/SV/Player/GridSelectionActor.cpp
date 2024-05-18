// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSelectionActor.h"
#include "../Utilities/SvUtilities.h"
#include "Components/DecalComponent.h"

// Sets default values
AGridSelectionActor::AGridSelectionActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("scene"));

	MouseGridSelectDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	MouseGridSelectDecal->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGridSelectionActor::BeginPlay()
{
	Super::BeginPlay();
	auto material = USvUtilities::GetMaterial("/Script/Engine.Material'/Game/Decals/Target_M.Target_M'");
	if (material) {
		MouseGridSelectDecal->SetMaterial(00, material);
	}

	MouseGridSelectDecal->SetRelativeRotation(FRotator(90, 0, 0));
	MouseGridSelectDecal->DecalSize = FVector(25, 50, 50);
}

