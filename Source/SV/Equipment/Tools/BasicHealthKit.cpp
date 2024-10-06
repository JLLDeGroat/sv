// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicHealthKit.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "../../Utilities/SvUtilities.h"
#include "Components/SceneComponent.h"

ABasicHealthKit::ABasicHealthKit(const FObjectInitializer& ObjectInitializer)
	: AEquipment(ObjectInitializer) {

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = RootSceneComponent;

	HealthKitMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KitMesh"));
	HealthKitMeshComponent->SetupAttachment(RootComponent);

	SprayEffectsComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Spray"));
	SprayEffectsComponent->SetupAttachment(RootComponent);

	auto effect = USvUtilities::GetNiagaraSystem("/Script/Niagara.NiagaraSystem'/Game/Effects/Equipment/BasicHealthKitSpray_N.BasicHealthKitSpray_N'");
	if (effect) {
		SprayEffectsComponent->SetAsset(effect);
	}

	auto mesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Equipment/Tools/MedKit.MedKit'");
	if (mesh) {
		HealthKitMeshComponent->SetStaticMesh(mesh);
	}

	SetActorScale3D(FVector(.08f));
}

void ABasicHealthKit::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	SprayEffectsComponent->SetRelativeLocation(FVector(36, 0, 53));
	SprayEffectsComponent->Deactivate();
}