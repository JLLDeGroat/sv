// Fill out your copyright notice in the Description page of Project Settings.


#include "CeilingStalagtite.h"
#include "../../Utilities/SvUtilities.h"

ACeilingStalagtite::ACeilingStalagtite(const FObjectInitializer& ObjectInitializer)
	: AEnvironmentActor(ObjectInitializer) {
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootSceneComponent;

	StalagtiteComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stalagtite"));
	StalagtiteComponent->SetupAttachment(RootComponent);
}

void ACeilingStalagtite::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	InitializeComponent();
}

void ACeilingStalagtite::InitializeComponent() {
	auto random = FMath::RandRange(1, 1001);

	UStaticMesh* mesh = nullptr;
	FVector offset = FVector::ZeroVector;
	FVector scale = FVector(FMath::RandRange(100, 165) / 100.0f);

	if (random > 750) {
		mesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Environment/Ceiling/Stalagtite2.Stalagtite2'");
		offset = FVector(0, 0, -300);
	}
	else if (random > 500) {
		mesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Environment/Ceiling/Stalagtite4.Stalagtite4'");
		offset = FVector(0, 0, -140);
	}
	else if (random > 250) {
		mesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Environment/Ceiling/Stalagtite5.Stalagtite5'");
		offset = FVector(0, 0, -300);
	}
	else {
		mesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Environment/Ceiling/Stalagtite7.Stalagtite7'");
		offset = FVector(0, 0, -300);
	}

	if (mesh) {
		StalagtiteComponent->SetStaticMesh(mesh);
		StalagtiteComponent->SetRelativeLocation(offset);
		StalagtiteComponent->SetRelativeScale3D(scale);
	}
}