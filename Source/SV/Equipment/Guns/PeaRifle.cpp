// Fill out your copyright notice in the Description page of Project Settings.


#include "PeaRifle.h"
#include "Components/StaticMeshComponent.h"
#include "Components/GunFireComponent.h"
#include "Components/LightAttachmentComponent.h"
#include "../../Utilities/SvUtilities.h"
#include "../Components/EquipmentDetailsComponent.h"
#include "../Components/AttachedVectorComponent.h"
#include "Components/MuzzleFlashComponent.h"
#include "../Bullets/Bullet.h"

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
	EquipmentDetailsComponent->SetMinBaseDamage(13);
	EquipmentDetailsComponent->SetMaxBaseDamage(37);
	EquipmentDetailsComponent->SetAccuracy(500);
	EquipmentDetailsComponent->SetAccuracyDecay(.075f);
	EquipmentDetailsComponent->SetBaseAccuracy(.35f);
	EquipmentDetailsComponent->SetMaxRounds(3);
	EquipmentDetailsComponent->SetReloadApCost(1);
	EquipmentDetailsComponent->SetGunType(EGun::G_PeaRifle);
	EquipmentDetailsComponent->SetCanOverwatch(true);
	EquipmentDetailsComponent->SetOverwatchApCost(2);
	EquipmentDetailsComponent->SetHolsterAttachType(EAttachType::AT_Backpack);
	EquipmentDetailsComponent->SetEquipSocket(EAttachType::AT_RightHand);

	AttachedVectorComponent->SetAttachedVectors(FVector(4, 2, 1), FRotator(0, 112.5, -173));
	AttachedVectorComponent->SetHolsteredVectors(FVector(0, 0, 0), FRotator(0, 100, 0));

	MuzzleFlashComponent = CreateDefaultSubobject<UMuzzleFlashComponent>(TEXT("MuzzleFlash"));
	MuzzleFlashComponent->SetupAttachment(GunMesh, FName("FireSocket"));
	MuzzleFlashComponent->SetRelativeRotation(FRotator(0, 90, 0));

	LightAttachmentComponent = CreateDefaultSubobject<ULightAttachmentComponent>(TEXT("LightAttachment"));
	LightAttachmentComponent->SetupAttachment(GunMesh, FName("LightAttachmentSocket"));

	LightAttachmentComponent->SetRelativeScale3D(FVector(.15f));
	LightAttachmentComponent->SetRelativeRotation(FRotator(0, -90, 0));
	LightAttachmentComponent->SetRelativeLocation(FVector(-2, 9, -2));
	LightAttachmentComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GunFireComponent->SetBulletClass(ABullet::StaticClass());

	MuzzleFlashComponent->SetFlashPelletColour(FLinearColor(FVector4(.76f, .69f, .29f, 1)));
	MuzzleFlashComponent->SetMuzzleFlashColour(FLinearColor(FVector4(.76f, .64f, .63f, 1)));
}

void APeaRifle::SetupAttachVector() {
	SetActorRelativeLocation(AttachedVectorComponent->GetAttachedLocation());
	SetActorRelativeRotation(AttachedVectorComponent->GetAttachedRotation());
}