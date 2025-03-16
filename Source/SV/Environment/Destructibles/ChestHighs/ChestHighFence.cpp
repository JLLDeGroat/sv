// Fill out your copyright notice in the Description page of Project Settings.

#include "ChestHighFence.h"
#include "GeometryCollection/GeometryCollection.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "../../../Utilities/SvUtilities.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Components/DestructibleDetails.h"
#include "../Components/DestructibleHitComponent.h"
#include "../../Components/VaultableComponent.h"

// Sets default values
AChestHighFence::AChestHighFence(const FObjectInitializer &ObjectInitializer) : ABaseChestHigh(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	auto restCollection = USvUtilities::GetGeometryCollection(GetRandomCollection());
	if (restCollection)
		DestructibleMesh->SetRestCollection(restCollection);

	Vaultable = CreateDefaultSubobject<UVaultableComponent>(TEXT("Vaultable"));
	Vaultable->SetupAttachment(RootComponent);

	bDisableAreaAtStart = false;
}

void AChestHighFence::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);
	DestructibleDetails->SetHealth(200);

	DisableBoxComponent->SetBoxExtent(FVector(2));

	//for reasons unknown, there is still a thirty odd unit falloff for this... so moving it far down has worked
	DisableBoxComponent->SetRelativeLocation(FVector(3, -5, -30));

	Vaultable->SetRelativeLocation(FVector(0, -50, 50));
	Vaultable->SetBoxExtent(FVector(20, 50, 32));
}

// Called when the game starts or when spawned
void AChestHighFence::BeginPlay()
{
	Super::BeginPlay();
}

FString AChestHighFence::GetRandomCollection()
{
	auto randomNum = FMath::RandRange(1, 5);
	UDebugMessages::LogError(this, "DEBUG ONLY USING ROCK 1");
	randomNum = 1;
	auto meshRef = "/Script/GeometryCollectionEngine.GeometryCollection'/Game/Environment/ChestHigh/CH_Fence/GC_CH_Fence" +
				   FString::SanitizeFloat(randomNum, 0) + ".GC_CH_Fence" + FString::SanitizeFloat(randomNum, 0) + "'";

	UDebugMessages::LogDisplay(this, meshRef);

	return meshRef;
}
