// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticEnemySpawner.h"
#include "../../Characters/Monsters/GruntConstruct.h"
#include "../../Characters/Monsters/RifleConstruct.h"
#include "../../Characters/Components/SpawnInComponent.h"
#include "../../Utilities/SvUtilities.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "NiagaraComponent.h"

AStaticEnemySpawner::AStaticEnemySpawner(const FObjectInitializer& ObjectInitializer)
	: AEnvironmentActor(ObjectInitializer) {

	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMeshComponent;
	BaseMeshComponent->SetCanEverAffectNavigation(false);
	BaseMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BaseMeshComponent->SetVisibility(false);

	SmokeComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SmokeComp"));
	SmokeComponent->SetupAttachment(BaseMeshComponent);

	auto baseMesh = USvUtilities::GetStaticMesh("/Script/Engine.StaticMesh'/Game/Environment/ExtractionItems/ExtractionLocationItem_MainBowl.ExtractionLocationItem_MainBowl'");
	if (baseMesh) {
		BaseMeshComponent->SetStaticMesh(baseMesh);
	}
}

void AStaticEnemySpawner::BeginPlay() {
	Super::BeginPlay();

	auto system = USvUtilities::GetNiagaraSystem("/Script/Niagara.NiagaraSystem'/Game/Effects/Environment/SmokePuff_N.SmokePuff_N'");
	if (system) {
		SmokeComponent->SetAsset(system);
		SmokeComponent->Deactivate();
	}
}

void AStaticEnemySpawner::StartSpawn() {

	auto random = FMath::RandRange(1, 101);
	UClass* characterClass = AGruntConstruct::StaticClass();

	if (random > 65)
		characterClass = ARifleConstruct::StaticClass();

	auto location = GetActorLocation();
	auto newGrunt = GetWorld()->SpawnActor<ABaseCharacter>(characterClass, location, FRotator(0));
	if (newGrunt) {
		auto spawnInComponent = newGrunt->GetComponentByClass<USpawnInComponent>();
		if (!spawnInComponent) {
			UDebugMessages::LogError(this, "enemy does not have spawn in component... deleting");
			newGrunt->Destroy();
			return;
		}
		spawnInComponent->StartSpawnInFromGround();
		SmokeComponent->Activate(true);
	}
}