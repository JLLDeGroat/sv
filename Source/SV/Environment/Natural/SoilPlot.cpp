// Fill out your copyright notice in the Description page of Project Settings.


#include "SoilPlot.h"
#include "../../Utilities/SvUtilities.h"


ASoilPlot::ASoilPlot(const FObjectInitializer& ObjectInitializer) 
	: AEnvironmentActor(ObjectInitializer) {


	SoilPlotMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Soil"));
	RootComponent = SoilPlotMeshComponent;

	SoilPlotMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASoilPlot::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	auto num = FMath::RandRange(1, 1001);
	
	auto reference = "/Script/Engine.StaticMesh'/Game/Environment/FarmPlot1.FarmPlot1'";
	
	if (num > 500) {
		reference = "/Script/Engine.StaticMesh'/Game/Environment/FarmPlot2.FarmPlot2'";
	}

	auto mesh = USvUtilities::GetStaticMesh(reference);
	if (mesh) {
		SoilPlotMeshComponent->SetStaticMesh(mesh);
	}
}