// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BulletCollisionComponent.h"
#include "Components/TravelComponent.h"
#include "Components/BulletDetailsComponent.h"
#include "../../Utilities/SvUtilities.h"
#include "Components/BulletTrailComponent.h"
#include "Components/BulletSoundComponent.h"
#include "Components/BulletHitSoundComponent.h"
#include "Components/BulletRearComponent.h"

// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BulletMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	RootComponent = BulletMeshComponent;
	BulletMeshComponent->SetCanEverAffectNavigation(false);
	BulletMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BulletCollisionComponent = CreateDefaultSubobject<UBulletCollisionComponent>(TEXT("BulletCollision"));
	BulletCollisionComponent->SetupAttachment(RootComponent);
	BulletCollisionComponent->SetRelativeLocation(FVector(-7, 0, 0));
	BulletCollisionComponent->SetCapsuleRadius(1, false);
	BulletCollisionComponent->SetCapsuleHalfHeight(10, false);
	BulletCollisionComponent->SetRelativeRotation(FRotator(90, 0, 0));

	TravelComponent = CreateDefaultSubobject<UTravelComponent>(TEXT("Travel"));

	auto bulletMesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Equipment/Bullets/Bullet.Bullet'");
	if (bulletMesh) {
		BulletMeshComponent->SetStaticMesh(bulletMesh);

		auto bulletMat = USvUtilities::GetMaterial("/Script/Engine.Material'/Game/Materials/Bullets/Bullet_M.Bullet_M'");
		BulletMeshComponent->SetMaterial(0, bulletMat);
	}

	BulletDetailsComponent = CreateDefaultSubobject<UBulletDetailsComponent>(TEXT("BulletDetails"));
	BulletTrailComponent = CreateDefaultSubobject<UBulletTrailComponent>(TEXT("BulletTrail"));
	BulletTrailComponent->SetupAttachment(RootComponent);


	BulletFireSoundComponent = CreateDefaultSubobject<UBulletSoundComponent>(TEXT("FireSound"));
	BulletHitSoundComponent = CreateDefaultSubobject<UBulletHitSoundComponent>(TEXT("HitSound"));
	BulletHitSoundComponent->SetSphereRadius(0);
	BulletFireSoundComponent->SetSphereRadius(0);

	BulletRearComponent = CreateDefaultSubobject<UBulletRearComponent>(TEXT("ReadComp"));
	BulletRearComponent->SetupAttachment(RootComponent);
	BulletRearComponent->SetSphereRadius(1);
	BulletRearComponent->SetRelativeLocation(FVector(-30, 0, 0));
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	//TODO Uncomment OnDebug
	GetWorld()->GetTimerManager().SetTimer(AutoDestroyTimer, this, &ABullet::OnAutoDestroyCallback, 10.0f, false);

	if (BulletFireSoundComponent) BulletFireSoundComponent->SetWorldLocation(GetActorLocation());
	if (BulletHitSoundComponent) BulletHitSoundComponent->SetWorldLocation(GetActorLocation());

	BulletFireSoundComponent->SetSphereRadius(800);
}

// Called every frame
void ABullet::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ABullet::OnAutoDestroyCallback() {
	Destroy();
}