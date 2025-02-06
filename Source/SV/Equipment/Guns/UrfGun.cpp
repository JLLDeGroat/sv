// Fill out your copyright notice in the Description page of Project Settings.

#include "UrfGun.h"
#include "Components/StaticMeshComponent.h"
#include "Components/GunFireComponent.h"
#include "Components/LightAttachmentComponent.h"
#include "../../Utilities/SvUtilities.h"
#include "../Components/EquipmentDetailsComponent.h"
#include "../Components/AttachedVectorComponent.h"
#include "Components/MuzzleFlashComponent.h"
#include "../Bullets/Bullet.h"

AUrfGun::AUrfGun(const FObjectInitializer &ObjectInitializer)
	: ABaseGun(ObjectInitializer)
{

	MeshComponent1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent1"));
	RootComponent = MeshComponent1;
	MeshComponent1->SetCanEverAffectNavigation(false);
	MeshComponent1->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MeshComponent2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent2"));
	MeshComponent2->SetupAttachment(RootComponent);
	MeshComponent2->SetCanEverAffectNavigation(false);
	MeshComponent2->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MeshComponent3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent3"));
	MeshComponent3->SetupAttachment(RootComponent);
	MeshComponent3->SetCanEverAffectNavigation(false);
	MeshComponent3->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MeshComponent4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent4"));
	MeshComponent4->SetupAttachment(RootComponent);
	MeshComponent4->SetCanEverAffectNavigation(false);
	MeshComponent4->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MeshComponent5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent5"));
	MeshComponent5->SetupAttachment(RootComponent);
	MeshComponent5->SetCanEverAffectNavigation(false);
	MeshComponent5->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GunFireComponent->SetMeshAndSocketName(MeshComponent1, "FireSocket");
	GunFireComponent->SetBulletClass(ABullet::StaticClass());

	EquipmentDetailsComponent->SetIsRange(true);
	EquipmentDetailsComponent->SetMinBaseDamage(35);
	EquipmentDetailsComponent->SetMaxBaseDamage(70);
	EquipmentDetailsComponent->SetAccuracy(300);
	EquipmentDetailsComponent->SetAccuracyDecay(.085f);
	EquipmentDetailsComponent->SetBaseAccuracy(.35f);
	EquipmentDetailsComponent->SetMaxRounds(3);
	EquipmentDetailsComponent->SetReloadApCost(1);
	EquipmentDetailsComponent->SetGunType(EGun::G_UrfGun);
	EquipmentDetailsComponent->SetCanOverwatch(true);
	EquipmentDetailsComponent->SetOverwatchApCost(1);
	EquipmentDetailsComponent->SetHolsterAttachType(EAttachType::AT_RightHolster);
	EquipmentDetailsComponent->SetEquipSocket(EAttachType::AT_RightHand);

	AttachedVectorComponent->SetAttachedVectors(FVector(6, 2, 1.5f), FRotator(0, 103, -175));
	AttachedVectorComponent->SetHolsteredVectors(FVector(13.0, -10, 0), FRotator(90, 0, 90));

	MuzzleFlashComponent = CreateDefaultSubobject<UMuzzleFlashComponent>(TEXT("MuzzleFlash"));
	MuzzleFlashComponent->SetupAttachment(MeshComponent1, FName("FireSocket"));
	MuzzleFlashComponent->SetRelativeRotation(FRotator(0, 90, 0));

	LightAttachmentComponent = CreateDefaultSubobject<ULightAttachmentComponent>(TEXT("LightAttachment"));
	LightAttachmentComponent->SetupAttachment(MeshComponent1, FName("LightAttachmentSocket"));

	LightAttachmentComponent->SetRelativeScale3D(FVector(.15f));
	LightAttachmentComponent->SetRelativeRotation(FRotator(0, -90, 0));
	LightAttachmentComponent->SetRelativeLocation(FVector(-2, 1, 1));

	LightAttachmentComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AUrfGun::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);

	auto mesh1 = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Equipment/1911_Barrel.1911_Barrel'");
	auto mesh2 = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Equipment/1911_Hammer.1911_Hammer'");
	auto mesh3 = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Equipment/1911_Mag.1911_Mag'");
	auto mesh4 = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Equipment/1911_Trigger.1911_Trigger'");
	auto mesh5 = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Equipment/1911_Mechanism.1911_Mechanism'");

	if (mesh1)
		MeshComponent1->SetStaticMesh(mesh1);
	if (mesh2)
		MeshComponent2->SetStaticMesh(mesh2);
	if (mesh3)
		MeshComponent3->SetStaticMesh(mesh3);
	if (mesh4)
		MeshComponent4->SetStaticMesh(mesh4);
	if (mesh5)
		MeshComponent5->SetStaticMesh(mesh5);
}