// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGenerationActors.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../SvUtilities.h"
#include "../../Runnables/LevelGenerationRunnable.h"
#include "../../Environment/EnvironmentActor.h"
#include "../../World/WorldGridItemActor.h"
#include "GameFramework/Character.h"
// Sets default values
AWorldGenerationActors::AWorldGenerationActors()
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


void AWorldGenerationActors::GenericLevel() {
	TearDownCurrentGen();

	auto i = FMath::RandRange(0, 999999);
	UDebugMessages::LogDisplay(this, "FMath::RandRange(0, 999999) = " + FString::SanitizeFloat(i));

	auto newLevel = NewObject<ULevelGenerationRunnable>()
		->InsertVariables(ELevelGenType::Generic)
		->Initialise(GetWorld(), i)
		->Begin();
}

void AWorldGenerationActors::TwoBuildingLevel() {
	TearDownCurrentGen();

	auto i = FMath::RandRange(0, 999999);
	UDebugMessages::LogDisplay(this, "FMath::RandRange(0, 999999) = " + FString::SanitizeFloat(i));

	if (BaseRunnable) {
		BaseRunnable->KillThread();
		BaseRunnable->EnsureCompletion();
	}

	BaseRunnable = NewObject<ULevelGenerationRunnable>()
		->InsertVariables(ELevelGenType::TwoBuilding)
		->Initialise(GetWorld(), i)
		->Begin();
}


#pragma optimize("", off)
void AWorldGenerationActors::TearDownCurrentGen() {
	auto actors = GetWorld()->GetCurrentLevel()->Actors;

	auto totalActors = actors.Num() - 1;
	for (int i = totalActors; i > 0; i--) {
		if (actors[i]) {
			if (actors[i]->IsA<AEnvironmentActor>()) {
				actors[i]->Destroy();
				continue;
			}
			if (actors[i]->IsA<ACharacter>()) {
				actors[i]->Destroy();
				continue;
			}
			if (actors[i]->IsA<AWorldGridItemActor>()) {
				actors[i]->Destroy();
				continue;
			}
		}
	}
}


void AWorldGenerationActors::BeginDestroy() {
	if (BaseRunnable) {
		BaseRunnable->KillThread();
		BaseRunnable->EnsureCompletion();
	}
	Super::BeginDestroy();
}
#pragma optimize("", on)