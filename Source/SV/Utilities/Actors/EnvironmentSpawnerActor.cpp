// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvironmentSpawnerActor.h"
#include "../SvUtilities.h"
#include "../GridUtilities.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../../Environment/Constructions/Interior/CornerTable.h"
#include "../../Environment/Constructions/Interior/LongTable.h"
#include "../../Environment/Constructions/Interior/RollingTable.h"
#include "../../Environment/Components/PotentialItemSpawnComponent.h"
#include "../../Environment/Pickups/IntelPickup.h"
#include "../../Environment/Indicators/PickupIndicator.h"
#include "../../Environment/Indicators/Components/IndicatorLinkComponent.h"

// Sets default values
AEnvironmentSpawnerActor::AEnvironmentSpawnerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootMesh->SetCanEverAffectNavigation(false);
	RootComponent = RootMesh;

	auto mesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'");
	if (mesh)
		RootMesh->SetStaticMesh(mesh);
}

// Called when the game starts or when spawned
void AEnvironmentSpawnerActor::BeginPlay()
{
	Super::BeginPlay();
	if (ConstructionType != EConstructionType::IT_NONE) {

		UClass* spawnClass = nullptr;
		FVector envSpawnLocation = GetActorLocation();
		switch (ConstructionType) {
		case EConstructionType::IT_CornerTable:
			spawnClass = ACornerTable::StaticClass();
			break;
		case EConstructionType::IT_RollingTable:
			spawnClass = ARollingTable::StaticClass();
			envSpawnLocation.Z = 20;
			envSpawnLocation.X -= 50;
			envSpawnLocation.Y -= 50;
			break;
		case EConstructionType::IT_LongTable:
			spawnClass = ALongTable::StaticClass();
			break;
		}

		if (spawnClass) {
			auto res = GetWorld()->SpawnActor<AEnvironmentActor>(spawnClass, envSpawnLocation, FRotator::ZeroRotator, FActorSpawnParameters());
			if (!res)
				UDebugMessages::LogError(this, "failed to spawn resource drop actor");
			else {
				if (bShouldSpawnIntel) {
					auto potentialItemSpawn = res->GetComponentByClass<UPotentialItemSpawnComponent>();
					if (potentialItemSpawn) {
						auto gridLocation = UGridUtilities::GetNormalisedGridLocation(GetActorLocation());

						auto itemLocation = potentialItemSpawn->GetComponentLocation();
						auto pickup = GetWorld()->SpawnActor<AIntelPickup>(itemLocation, FRotator::ZeroRotator);

						if (pickup) {
							TArray<FVector> adjacentTiles;
							USvUtilities::GetAdjacentGridTiles(gridLocation, adjacentTiles);

							bool spawnedIndicator = false;
							for (int i = 0; i < adjacentTiles.Num(); i++) {
								FHitResult EnvironmentHit;
								GetWorld()->LineTraceSingleByChannel(EnvironmentHit, gridLocation, adjacentTiles[i], USvUtilities::GetEnvironmentChannel());

								//if (!EnvironmentHit.bBlockingHit && !spawnedIndicator) {
								if (!EnvironmentHit.bBlockingHit) {
									auto indicatorSpawnLocation = adjacentTiles[i];
									indicatorSpawnLocation.Z = -5;

									spawnedIndicator = true;
									auto indicatorActor = GetWorld()->SpawnActor<APickupIndicator>(indicatorSpawnLocation, FRotator::ZeroRotator);
									if (indicatorActor) {
										UDebugMessages::LogDisplay(this, "set actor indicator");

										auto linkComponent = indicatorActor->GetComponentByClass<UIndicatorLinkComponent>();
										if (linkComponent) {
											linkComponent->SetPickupIndicatingTo(pickup);
										}
									}
								}
							}
						}
					}
					else UDebugMessages::LogError(this, "there is no potential item spawn component");
				}
			}
		}
	}

	Destroy();
}

// Called every frame
void AEnvironmentSpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

