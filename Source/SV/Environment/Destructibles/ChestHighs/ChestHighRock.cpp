// Fill out your copyright notice in the Description page of Project Settings.

#include "ChestHighRock.h"
#include "GeometryCollection/GeometryCollection.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "../../../Utilities/SvUtilities.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "PhysicsField/PhysicsFieldComponent.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemNodes.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "../Components/DestructibleDetails.h"
#include "../Components/DestructibleHitComponent.h"
#include "../../Components/SkippableComponent.h"

// Sets default values
AChestHighRock::AChestHighRock(const FObjectInitializer &ObjectInitializer) : ABaseChestHigh(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	auto restCollection = USvUtilities::GetGeometryCollection(GetRandomCollection());
	if (restCollection)
		DestructibleMesh->SetRestCollection(restCollection);

	DestructibleMesh->SetCollisionObjectType(USvUtilities::GetEnvironmentChannel());
	DestructibleMesh->SetCollisionResponseToChannel(USvUtilities::GetEnvironmentChannel(), ECR_Block);
	DestructibleMesh->SetCollisionResponseToChannel(USvUtilities::GetBulletCollisionChannel(), ECR_Overlap);
	DestructibleMesh->SetCollisionResponseToChannel(USvUtilities::GetBulletCollisionObjectChannel(), ECR_Overlap);
	DestructibleMesh->SetGenerateOverlapEvents(true);

	Skippable = CreateDefaultSubobject<USkippableComponent>(TEXT("Skippable"));
	Skippable->SetupAttachment(RootComponent);

	DisableBoxComponent->SetBoxExtent(FVector(50, 50, 50));
	DisableBoxComponent->SetRelativeLocation(FVector(50, -50, -50));

	DestructibleDetails->SetHealth(300);
	Skippable->SetRelativeLocation(FVector(50, -50, 50));
	Skippable->SetBoxExtent(FVector(40, 40, 32));

	DisableBoxComponent->bHiddenInGame = false;
	Skippable->bHiddenInGame = false;
}

void AChestHighRock::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);
}

// Called when the game starts or when spawned
void AChestHighRock::BeginPlay()
{
	Super::BeginPlay();
	DestructibleMesh->SetRelativeRotation(FRotator(0, FMath::RandRange(0, 180), 0));
}

FString AChestHighRock::GetRandomCollection()
{
	auto randomNum = FMath::RandRange(1, 5);
	auto meshRef = "/Script/GeometryCollectionEngine.GeometryCollection'/Game/Environment/ChestHigh/CH_Rocks/GC_CH_Rock" +
				   FString::SanitizeFloat(randomNum, 0) + ".GC_CH_Rock" + FString::SanitizeFloat(randomNum, 0) + "'";

	UDebugMessages::LogDisplay(this, meshRef);

	return meshRef;
}
