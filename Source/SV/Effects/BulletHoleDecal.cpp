// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletHoleDecal.h"
#include "../Utilities/SvUtilities.h"
#include "NiagaraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/SceneComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "VgCore/Domain/Debug/DebugMessages.h"

ABulletHoleDecal::ABulletHoleDecal(const FObjectInitializer& ObjectInitializer)
	: AActor(ObjectInitializer) {

	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneRootComponent;

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("BulletHoleDecal"));
	DecalComponent->SetupAttachment(RootComponent);
	DecalComponent->SetRelativeScale3D(FVector(.03f));
	DecalComponent->SetRelativeRotation(FRotator(0, 180, 0));
	DecalComponent->SetDecalMaterial(USvUtilities::GetBulletHoleMaterial());

	BulletShavingsComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BulletShavings"));
	BulletShavingsComponent->SetupAttachment(RootComponent);
	BulletShavingsComponent->SetRelativeLocation(FVector(10, 0, 0));
	PrimaryActorTick.bCanEverTick = true;
}

void ABulletHoleDecal::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	auto asset = USvUtilities::GetNiagaraSystem("/Script/Niagara.NiagaraSystem'/Game/Effects/BulletShrapnelBurst_N.BulletShrapnelBurst_N'");
	if (asset)
		BulletShavingsComponent->SetAsset(asset);
}

void ABulletHoleDecal::BeginPlay() {
	Super::BeginPlay();

	DecalDynamicMaterial = DecalComponent->CreateDynamicMaterialInstance();
	EmissionDecayValue = 0;
	auto current = DecalComponent->GetRelativeRotation();
	DecalComponent->SetRelativeRotation(FRotator(current.Pitch, current.Yaw, FMath::RandRange(0, 360)));
	SetActorTickEnabled(true);
	BulletShavingsComponent->Activate(true);
}
void ABulletHoleDecal::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	EmissionDecayValue += .005f;

	if (DecalDynamicMaterial)
		DecalDynamicMaterial->SetScalarParameterValue(FName("EmissionDecay"), EmissionDecayValue);

	if (EmissionDecayValue >= 1) {
		SetActorTickEnabled(false);
		GetWorld()->GetTimerManager().SetTimer(DestroyHandle, this, &ABulletHoleDecal::DestroyCallback, 5.0f);
	}
}

void ABulletHoleDecal::DestroyCallback() {
	Destroy();
}