// Fill out your copyright notice in the Description page of Project Settings.


#include "DirectionOptionActor.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "../Utilities/SvUtilities.h"

// Sets default values
ADirectionOptionActor::ADirectionOptionActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ClickBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = ClickBoxComponent;
	ClickBoxComponent->SetCollisionObjectType(USvUtilities::GetWorldSelectChannel());
	ClickBoxComponent->SetGenerateOverlapEvents(true);
	ClickBoxComponent->SetCollisionResponseToChannel(USvUtilities::GetWorldSelectChannel(), ECR_Block);

	ArrowDecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("ArrowDecal"));
	ArrowDecalComponent->SetupAttachment(RootComponent);
	ArrowDecalComponent->SetRelativeRotation(FRotator(90, 0, 0));
	ArrowDecalComponent->SetRelativeScale3D(FVector(.15f));

	auto material = USvUtilities::GetMaterial("/Script/Engine.Material'/Game/Decals/DirectionDecal_M.DirectionDecal_M'");
	DynamicInstance = UMaterialInstanceDynamic::Create(material, this);

	DynamicInstance->SetScalarParameterValue("Emission", 1.5f);
	DynamicInstance->SetVectorParameterValue("DecalColour", FVector(0, 0.771772f, .8333333f));
	if (material)
		ArrowDecalComponent->SetMaterial(0, DynamicInstance);
}

// Called when the game starts or when spawned
void ADirectionOptionActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADirectionOptionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADirectionOptionActor::ActivateArrowEmission() {
	DynamicInstance->SetScalarParameterValue("Emission", 5);
}
void ADirectionOptionActor::DeactivateArrowEmission() {
	DynamicInstance->SetScalarParameterValue("Emission", 1.5f);
}

