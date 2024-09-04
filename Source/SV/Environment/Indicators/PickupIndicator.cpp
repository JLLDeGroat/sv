// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupIndicator.h"
#include "Components/IndicatorActivatorComponent.h"
#include "Components/IndicatorLinkComponent.h"
#include "../../Utilities/SvUtilities.h"
// Sets default values
APickupIndicator::APickupIndicator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMeshComponent;

	auto baseMesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Environment/ExtractionItems/ExtractionLocationItem_MainBowl.ExtractionLocationItem_MainBowl'");
	if (baseMesh) {
		BaseMeshComponent->SetStaticMesh(baseMesh);
	}
	BaseMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ActivatorIndicator = CreateDefaultSubobject<UIndicatorActivatorComponent>(TEXT("Indicator"));
	ActivatorIndicator->SetupAttachment(RootComponent);
	ActivatorIndicator->SetBoxExtent(FVector(20, 20, 32));
	ActivatorIndicator->SetRelativeLocation(FVector(0, 0, 35));
	ActivatorIndicator->SetIndicatorType(EIndicatorType::IT_Interact);

	LinkComponent = CreateDefaultSubobject<UIndicatorLinkComponent>(TEXT("LinkComponent"));
}
// Called when the game starts or when spawned
void APickupIndicator::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APickupIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

