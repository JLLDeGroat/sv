// Fill out your copyright notice in the Description page of Project Settings.

#include "ConstructRifle.h"
#include "../../Utilities/SvUtilities.h"
#include "../Components/EquipmentDetailsComponent.h"
#include "../Components/AttachedVectorComponent.h"
#include "Components/GunFireComponent.h"
#include "Components/MuzzleFlashComponent.h"
#include "../Bullets/PlasmaBullet.h"
#include "Components/GunActivationComponent.h"

AConstructRifle::AConstructRifle(const FObjectInitializer &ObjectInitializer) : ABaseGun(ObjectInitializer)
{

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));

	BackSectionComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackMesh"));
	CannonSectionComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	MidSectionComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mid1Mesh"));
	MidSection2Component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mid2Mesh"));
	MidSection3Component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mid3Mesh"));
	MidSection4Component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mid4Mesh"));
	BackConnectorComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackConMesh"));
	FrontConnectorComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrontConMesh"));

	RootComponent = RootSceneComponent;
	BackSectionComponent->SetupAttachment(RootComponent);
	CannonSectionComponent->SetupAttachment(RootComponent);

	MidSectionComponent->SetupAttachment(RootComponent);
	MidSection2Component->SetupAttachment(RootComponent);
	MidSection3Component->SetupAttachment(RootComponent);
	MidSection4Component->SetupAttachment(RootComponent);

	BackConnectorComponent->SetupAttachment(RootComponent);
	FrontConnectorComponent->SetupAttachment(RootComponent);

	auto backMesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Equipment/ConstructCannon/Construct_Gun_BackSection.Construct_Gun_BackSection'");
	if (backMesh)
		BackSectionComponent->SetStaticMesh(backMesh);

	auto cannonMesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Equipment/ConstructCannon/Construct_Gun_Cannon.Construct_Gun_Cannon'");
	auto midSection = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Equipment/ConstructCannon/Construct_Gun_MidSection.Construct_Gun_MidSection'");
	auto connection = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Equipment/ConstructCannon/Construct_Gun_Connector.Construct_Gun_Connector'");

	if (cannonMesh)
		CannonSectionComponent->SetStaticMesh(cannonMesh);

	if (midSection)
	{
		MidSectionComponent->SetStaticMesh(midSection);
		MidSection2Component->SetStaticMesh(midSection);
		MidSection3Component->SetStaticMesh(midSection);
		MidSection4Component->SetStaticMesh(midSection);
	}

	if (connection)
	{
		FrontConnectorComponent->SetStaticMesh(connection);
		BackConnectorComponent->SetStaticMesh(connection);
	}

	BackSectionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BackSectionComponent->SetCanEverAffectNavigation(false);

	CannonSectionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CannonSectionComponent->SetCanEverAffectNavigation(false);

	MidSectionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MidSectionComponent->SetCanEverAffectNavigation(false);
	MidSection2Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MidSection2Component->SetCanEverAffectNavigation(false);
	MidSection3Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MidSection3Component->SetCanEverAffectNavigation(false);
	MidSection4Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MidSection4Component->SetCanEverAffectNavigation(false);

	BackConnectorComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BackConnectorComponent->SetCanEverAffectNavigation(false);
	FrontConnectorComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FrontConnectorComponent->SetCanEverAffectNavigation(false);

	BackSectionComponent->SetRelativeLocation(FVector(-157, 0, 0));
	CannonSectionComponent->SetRelativeLocation(FVector(145, 0, 0));

	MidSectionComponent->SetRelativeLocation(FVector(-80, 0, 0));
	MidSection2Component->SetRelativeLocation(FVector(-30, 0, 0));
	MidSection3Component->SetRelativeLocation(FVector(20, 0, 0));
	MidSection4Component->SetRelativeLocation(FVector(70, 0, 0));

	BackConnectorComponent->SetRelativeLocation(FVector(-130, 0, 40));
	FrontConnectorComponent->SetRelativeLocation(FVector(0, 0, 40));

	EquipmentDetailsComponent->SetIsRange(true);
	EquipmentDetailsComponent->SetMinBaseDamage(5);
	EquipmentDetailsComponent->SetMaxBaseDamage(20);
	EquipmentDetailsComponent->SetAccuracy(400);
	EquipmentDetailsComponent->SetAccuracyDecay(.075f);
	EquipmentDetailsComponent->SetBaseAccuracy(.35f);
	EquipmentDetailsComponent->SetMaxRounds(5);
	EquipmentDetailsComponent->SetReloadApCost(1);
	EquipmentDetailsComponent->SetGunType(EGun::G_ConstructRifle);
	EquipmentDetailsComponent->SetCanOverwatch(true);
	EquipmentDetailsComponent->SetOverwatchApCost(2);
	EquipmentDetailsComponent->SetHolsterAttachType(EAttachType::INVALID);
	EquipmentDetailsComponent->SetEquipSocket(EAttachType::AT_RightArm);
	EquipmentDetailsComponent->SetRelativeScaleOnEquip(FVector(.15f));

	AttachedVectorComponent->SetAttachedVectors(FVector(30, 3, 10), FRotator(-5, 0, 180));
	AttachedVectorComponent->SetHolsteredVectors(FVector(0, 0, 0), FRotator(0, 0, 0));

	GunFireComponent->SetMeshAndSocketName(CannonSectionComponent, "FireSocket");
	GunFireComponent->SetBulletClass(APlasmaBullet::StaticClass());

	MuzzleFlashComponent = CreateDefaultSubobject<UMuzzleFlashComponent>(TEXT("MuzzleFlash"));
	MuzzleFlashComponent->SetupAttachment(CannonSectionComponent, FName("FireSocket"));

	ActivationComponent = CreateDefaultSubobject<UGunActivationComponent>(TEXT("Activation"));

	TArray<UStaticMeshComponent *> spinupMeshes;
	spinupMeshes.Emplace(MidSectionComponent);
	spinupMeshes.Emplace(MidSection2Component);
	spinupMeshes.Emplace(MidSection3Component);
	spinupMeshes.Emplace(MidSection4Component);
	ActivationComponent->SetupForEngineSpinUpActivation(spinupMeshes);

	MuzzleFlashComponent->SetFlashPelletColour(FLinearColor(FVector4(.75f, .11f, .56f, 1)));
	MuzzleFlashComponent->SetMuzzleFlashColour(FLinearColor(FVector4(.75f, .11f, .56f, 1)));
	MuzzleFlashComponent->SetMuzzleFlashSystem("/Script/Niagara.NiagaraSystem'/Game/Effects/Trails/PlasmaFlash_N.PlasmaFlash_N'");
}

void AConstructRifle::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);

	// MuzzleFlashComponent->SetVariableVec4(FName("MuzzleFlash"), FVector4(.75f, .11f, .56f, 1));
	// auto flashComp = MuzzleFlashComponent->GetFlashHeatPelletComponent();
	// flashComp->SetVariableVec4(FName("FlashColour"), FVector4(.75f, .11f, .56f, 1));
}