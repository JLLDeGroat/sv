// Fill out your copyright notice in the Description page of Project Settings.


#include "PlasmaBullet.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BulletCollisionComponent.h"
#include "Components/TravelComponent.h"
#include "Components/BulletDetailsComponent.h"
#include "../../Utilities/SvUtilities.h"
#include "Components/BulletTrailComponent.h"
#include "Components/BulletSoundComponent.h"
#include "Components/BulletHitSoundComponent.h"
#include "Components/BulletRearComponent.h"
#include "Components/BulletAuraComponent.h"

APlasmaBullet::APlasmaBullet(const FObjectInitializer& ObjectInitializer)
	: ABaseBullet(ObjectInitializer) {
	PrimaryActorTick.bCanEverTick = false;

	BulletCollisionComponent->SetRelativeLocation(FVector(-7, 0, 0));
	BulletCollisionComponent->SetCapsuleRadius(1, false);
	BulletCollisionComponent->SetCapsuleHalfHeight(10, false);
	BulletCollisionComponent->SetRelativeRotation(FRotator(90, 0, 0));

	auto bulletMesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Equipment/Bullets/Bullet.Bullet'");
	if (bulletMesh) {
		BulletMeshComponent->SetStaticMesh(bulletMesh);

		auto bulletMat = USvUtilities::GetMaterial("/Script/Engine.Material'/Game/Materials/Bullets/Bullet_M.Bullet_M'");
		BulletMeshComponent->SetMaterial(0, bulletMat);
	}

	BulletRearComponent->SetSphereRadius(1);
	BulletRearComponent->SetRelativeLocation(FVector(-30, 0, 0));

	BulletAuraComponent = CreateDefaultSubobject<UBulletAuraComponent>(TEXT("BulletAura"));
	BulletAuraComponent->SetupAttachment(RootComponent);
	
	auto basicAssetRef = "/Script/Niagara.NiagaraSystem'/Game/Effects/PlasmaAura_N.PlasmaAura_N'";
	auto bulletAura = USvUtilities::GetNiagaraSystem(basicAssetRef);
	if (bulletAura) {
		BulletAuraComponent->SetAsset(bulletAura);
	}

	auto bulletTrailRef = "/Script/Niagara.NiagaraSystem'/Game/Effects/Trails/PlasmaBulletTrail_N.PlasmaBulletTrail_N'";
	auto plasmaTrailSystem = USvUtilities::GetNiagaraSystem(bulletTrailRef);
	if (plasmaTrailSystem) {
		BulletTrailComponent->SetAsset(plasmaTrailSystem);
	}
}