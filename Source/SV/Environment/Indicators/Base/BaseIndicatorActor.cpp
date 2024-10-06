// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseIndicatorActor.h"
#include "../Components/IndicatorActivatorComponent.h"
#include "../../../Utilities/SvUtilities.h"

// Sets default values
ABaseIndicatorActor::ABaseIndicatorActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMeshComponent;

	auto baseMesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Environment/ExtractionItems/ExtractionLocationItem_MainBowl.ExtractionLocationItem_MainBowl'");
	if (baseMesh) {
		BaseMeshComponent->SetStaticMesh(baseMesh);
		BaseMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	ActivatorIndicator = CreateDefaultSubobject<UIndicatorActivatorComponent>(TEXT("Indicator"));
	ActivatorIndicator->SetupAttachment(RootComponent);
	ActivatorIndicator->SetBoxExtent(FVector(20, 20, 32));
	ActivatorIndicator->SetRelativeLocation(FVector(0, 0, 35));

	auto material = USvUtilities::GetMaterial("/Script/Engine.Material'/Game/Materials/ExtractionIndicator_M.ExtractionIndicator_M'");
	IndicatorMaterial = UMaterialInstanceDynamic::Create(material, this);
	BaseMeshComponent->SetMaterial(0, IndicatorMaterial);
}

void ABaseIndicatorActor::SetMaterialColour(FLinearColor color) {
	IndicatorMaterial->SetVectorParameterValue("IndicatorColour", color);
	if (ActivatorIndicator)
		ActivatorIndicator->SetMaterialColour(color);
}

// Called when the game starts or when spawned
void ABaseIndicatorActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABaseIndicatorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

