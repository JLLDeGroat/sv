// Fill out your copyright notice in the Description page of Project Settings.


#include "PeaRifle.h"
#include "Components/StaticMeshComponent.h"
#include "Components/GunFireComponent.h"
#include "../../Utilities/SvUtilities.h"

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
}

void APeaRifle::SetupAttachVector() {
	SetActorRelativeLocation(FVector(5, 2, 0));
	SetActorRelativeRotation(FRotator(0, 112.5, 182));
}