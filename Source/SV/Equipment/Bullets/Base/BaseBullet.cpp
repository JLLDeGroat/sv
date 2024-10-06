// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBullet.h"
#include "Components/StaticMeshComponent.h"
#include "../Components/BulletCollisionComponent.h"
#include "../Components/TravelComponent.h"
#include "../Components/BulletDetailsComponent.h"
#include "../../../Utilities/SvUtilities.h"
#include "../Components/BulletTrailComponent.h"
#include "../Components/BulletSoundComponent.h"
#include "../Components/BulletHitSoundComponent.h"
#include "../Components/BulletRearComponent.h"

// Sets default values
ABaseBullet::ABaseBullet(const FObjectInitializer& ObjectInitializer)
	: AActor(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	BulletMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	RootComponent = BulletMeshComponent;
	BulletMeshComponent->SetCanEverAffectNavigation(false);
	BulletMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BulletCollisionComponent = CreateDefaultSubobject<UBulletCollisionComponent>(TEXT("BulletCollision"));
	BulletCollisionComponent->SetupAttachment(RootComponent);

	TravelComponent = CreateDefaultSubobject<UTravelComponent>(TEXT("Travel"));

	BulletDetailsComponent = CreateDefaultSubobject<UBulletDetailsComponent>(TEXT("BulletDetails"));
	BulletTrailComponent = CreateDefaultSubobject<UBulletTrailComponent>(TEXT("BulletTrail"));
	BulletTrailComponent->SetupAttachment(RootComponent);

	BulletFireSoundComponent = CreateDefaultSubobject<UBulletSoundComponent>(TEXT("FireSound"));
	BulletHitSoundComponent = CreateDefaultSubobject<UBulletHitSoundComponent>(TEXT("HitSound"));
	BulletHitSoundComponent->SetSphereRadius(0);
	BulletFireSoundComponent->SetSphereRadius(0);

	BulletRearComponent = CreateDefaultSubobject<UBulletRearComponent>(TEXT("ReadComp"));
	BulletRearComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABaseBullet::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABaseBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseBullet::OnAutoDestroyCallback() {
	Destroy();
}

