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

