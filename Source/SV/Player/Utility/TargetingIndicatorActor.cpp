// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetingIndicatorActor.h"
#include "Components/SphereComponent.h"
#include "../../Utilities/SvUtilities.h"

// Sets default values
ATargetingIndicatorActor::ATargetingIndicatorActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = SphereComponent;

	/*SphereComponent->SetGenerateOverlapEvents(true);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AGrenadeIndicatorActor::Overlapped);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AGrenadeIndicatorActor::OverlapEnded);*/

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCanEverAffectNavigation(false);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	auto mesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'");
	auto material = USvUtilities::GetMaterial("/Script/Engine.Material'/Game/Materials/GrenadeIndicator_M.GrenadeIndicator_M'");
	if (mesh && material) {
		MeshComponent->SetStaticMesh(mesh);
		MeshComponent->SetMaterial(0, material);
		MeshComponent->SetRelativeScale3D(FVector(.65f));
	}

}

// Called when the game starts or when spawned
void ATargetingIndicatorActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATargetingIndicatorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ATargetingIndicatorActor::GetSphereScaledRadius() {
	return SphereComponent->GetScaledSphereRadius();
}

