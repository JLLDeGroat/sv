// Fill out your copyright notice in the Description page of Project Settings.


#include "PeaRifle.h"
#include "Components/StaticMeshComponent.h"
#include "Components/GunFireComponent.h"
#include "Components/LightAttachmentComponent.h"
#include "../../Utilities/SvUtilities.h"
#include "../Components/EquipmentDetailsComponent.h"
#include "../Components/AttachedVectorComponent.h"
#include "Components/MuzzleFlashComponent.h"

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
	EquipmentDetailsComponent->SetAccuracy(500);
	EquipmentDetailsComponent->SetAccuracyDecay(.2f);
	EquipmentDetailsComponent->SetBaseAccuracy(.35f);

	AttachedVectorComponent->SetAttachedVectors(FVector(4, 2, 0), FRotator(0, 112.5, 182));
	AttachedVectorComponent->SetHolsteredVectors(FVector(0, 0, 0), FRotator(0, 100, 0));

	MuzzleFlashComponent = CreateDefaultSubobject<UMuzzleFlashComponent>(TEXT("MuzzleFlash"));
	MuzzleFlashComponent->SetupAttachment(GunMesh, FName("FireSocket"));
	MuzzleFlashComponent->SetRelativeRotation(FRotator(0, 90, 0));

	LightAttachmentComponent = CreateDefaultSubobject<ULightAttachmentComponent>(TEXT("LightAttachment"));
	LightAttachmentComponent->SetupAttachment(GunMesh, FName("LightAttachmentSocket"));

	LightAttachmentComponent->SetRelativeScale3D(FVector(.15f));
	LightAttachmentComponent->SetRelativeRotation(FRotator(0, -90, 0));
	LightAttachmentComponent->SetRelativeLocation(FVector(-2, 9, -2));
}

void APeaRifle::SetupAttachVector() {
	SetActorRelativeLocation(AttachedVectorComponent->GetAttachedLocation());
	SetActorRelativeRotation(AttachedVectorComponent->GetAttachedRotation());
}