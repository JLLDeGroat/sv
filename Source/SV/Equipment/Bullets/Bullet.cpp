// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BulletCollisionComponent.h"
#include "Components/TravelComponent.h"
#include "Components/BulletDetailsComponent.h"
#include "../../Utilities/SvUtilities.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BulletCollisionComponent = CreateDefaultSubobject<UBulletCollisionComponent>(TEXT("Collision"));
	RootComponent = BulletCollisionComponent;

	BulletCollisionComponent->SetCapsuleRadius(1, false);
	BulletCollisionComponent->SetCapsuleHalfHeight(10, false);
	BulletCollisionComponent->SetRelativeRotation(FRotator(90, 0, 0));

	BulletMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMeshComponent->SetupAttachment(RootComponent);
	BulletMeshComponent->SetRelativeLocation(FVector(0, 0, -7));
	BulletMeshComponent->SetRelativeRotation(FRotator(-90, 0, 0));
	BulletMeshComponent->SetCanEverAffectNavigation(false);
	BulletMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	TravelComponent = CreateDefaultSubobject<UTravelComponent>(TEXT("Travel"));

	auto bulletMesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Equipment/Bullets/Bullet.Bullet'");
	if (bulletMesh) {
		BulletMeshComponent->SetStaticMesh(bulletMesh);

		auto bulletMat = USvUtilities::GetMaterial("/Script/Engine.Material'/Game/Materials/Bullets/Bullet_M.Bullet_M'");
		BulletMeshComponent->SetMaterial(0, bulletMat);
	}

	BulletDetailsComponent = CreateDefaultSubobject<UBulletDetailsComponent>(TEXT("BulletDetails"));
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	//TODO Uncomment OnDebug
	//GetWorld()->GetTimerManager().SetTimer(AutoDestroyTimer, this, &ABullet::OnAutoDestroyCallback, 20.0f, false);
}

// Called every frame
void ABullet::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ABullet::OnAutoDestroyCallback() {
	Destroy();
}