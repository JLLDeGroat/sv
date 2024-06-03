// Fill out your copyright notice in the Description page of Project Settings.


#include "PeaRifle.h"
#include "Components/StaticMeshComponent.h"
#include "Components/GunFireComponent.h"
#include "../../Utilities/SvUtilities.h"
#include "../Components/EquipmentDetailsComponent.h"
#include "../Components/AttachedVectorComponent.h"

APeaRifle::APeaRifle(const FObjectInitializer& ObjectInitializer) : AEquipment(ObjectInitializer) {

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
	RootComponent = GunMesh;

	auto mesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Equipment/PeaRifle.PeaRifle'");
	if (mesh) {
		GunMesh->SetStaticMesh(mesh);
		GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GunMesh->SetCanEverAffectNavigation(false);
	}

	GunFireComponent = CreateDefaultSubobject<UGunFireComponent>(TEXT("GunFire"));
	GunFireComponent->SetMeshAndSocketName(GunMesh, "FireSocket");

	EquipmentDetailsComponent->SetIsRange(true);
	EquipmentDetailsComponent->SetBaseDamage(25);

	AttachedVectorComponent->SetAttachedVectors(FVector(5, 2, 0), FRotator(0, 112.5, 182));
	AttachedVectorComponent->SetHolsteredVectors(FVector(0, 0, 0), FRotator(0, 100, 0));
}

void APeaRifle::SetupAttachVector() {
	SetActorRelativeLocation(AttachedVectorComponent->GetAttachedLocation());
	SetActorRelativeRotation(AttachedVectorComponent->GetAttachedRotation());
}