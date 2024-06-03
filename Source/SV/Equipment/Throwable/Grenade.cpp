// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "../Components/EquipmentDetailsComponent.h"
#include "Components/ThrowTravelComponent.h"
#include "../../Utilities/SvUtilities.h"
#include "Components/CapsuleComponent.h"
#include "Components/ThrowExplosionComponent.h"
#include "../Components/AttachedVectorComponent.h"


AGrenade::AGrenade(const FObjectInitializer& ObjectInitializer)
	: AEquipment(ObjectInitializer) {

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;
	CapsuleComponent->InitCapsuleSize(4, 7);

	GrenadeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	GrenadeMesh->SetupAttachment(RootComponent);

	auto mesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Equipment/Throwables/Grenade.Grenade'");
	if (mesh) {
		GrenadeMesh->SetStaticMesh(mesh);
		GrenadeMesh->SetCanEverAffectNavigation(false);
		GrenadeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GrenadeMesh->SetRelativeScale3D(FVector(0.05f));
	}

	TravelComponent = CreateDefaultSubobject<UThrowTravelComponent>(TEXT("Travel"));
	ExplosionComponent = CreateDefaultSubobject<UThrowExplosionComponent>(TEXT("Explosion"));
	ExplosionComponent->SetupAttachment(RootComponent);

	AttachedVectorComponent->SetAttachedVectors(FVector::ZeroVector, FRotator(219, -5, 0));
}