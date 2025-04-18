// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldGenerationActors.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../SvUtilities.h"
#include "../../Runnables/LevelGenerationRunnable.h"
#include "../../Runnables/Checkers/WinLossCheckerRunnable.h"
#include "../../Environment/EnvironmentActor.h"
#include "../../World/WorldGridItemActor.h"
#include "../../Environment/Ceiling/CeilingAmbiantLight.h"
#include "../../Environment/Ceiling/CeilingStalagtite.h"
#include "../../Environment/Flooring/MudFloor.h"
#include "../../Environment/Prefabs/Base/BasePrefab.h"
#include "../../Environment/Fog/FogManager.h"
#include "../../GameModes/GameplayMode.h"
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

	bShouldGenerate = true;
}

void AWorldGenerationActors::BeginPlay()
{
	Super::BeginPlay();
	auto gameMode = GetWorld()->GetAuthGameMode<AGameplayMode>();
	if (!gameMode)
	{
		UDebugMessages::LogDisplay(this, "game mode is null, not generating");
		return;
	}
	if (bShouldGenerate)
	{
		TearDownCurrentGen();
		auto newLevel = NewObject<ULevelGenerationRunnable>()
							->InsertVariables(ELevelGenType::TwoBuilding)
							->Initialise(GetWorld())
							->Begin();
	}

	if (bShouldGenerateFog)
		GenerateFog();
}

void AWorldGenerationActors::GenericLevel()
{
	if (!bShouldGenerate)
	{
		UDebugMessages::LogWarning(this, "Should Generate is set to false");
		return;
	}
	TearDownCurrentGen();

	if (BaseRunnable)
	{
		BaseRunnable->KillThread();
		BaseRunnable->EnsureCompletion();
	}

	BaseRunnable = NewObject<ULevelGenerationRunnable>()
					   ->InsertVariables(ELevelGenType::Generic)
					   ->Initialise(GetWorld(), FMath::RandRange(0, 999999))
					   ->Begin();
}

void AWorldGenerationActors::TwoBuildingLevel()
{
	if (!bShouldGenerate)
	{
		UDebugMessages::LogWarning(this, "Should Generate is set to false");
		return;
	}
	TearDownCurrentGen();

	if (BaseRunnable)
	{
		BaseRunnable->KillThread();
		BaseRunnable->EnsureCompletion();
	}

	BaseRunnable = NewObject<ULevelGenerationRunnable>()
					   ->InsertVariables(ELevelGenType::TwoBuilding)
					   ->Initialise(GetWorld(), FMath::RandRange(0, 999999))
					   ->Begin();
}
#pragma optimize("", off)
void AWorldGenerationActors::TearDownCurrentGen()
{
	auto actors = GetWorld()->GetCurrentLevel()->Actors;

	auto totalActors = actors.Num() - 1;
	for (int i = totalActors; i > 0; i--)
	{
		if (actors[i])
		{
			if (actors[i]->IsA<ABasePrefab>())
			{
				auto thisActor = actors[i];
				for (UActorComponent *comp : actors[i]->GetComponents())
				{
					comp->DestroyComponent();
				}
				TArray<AActor *> childActors;
				actors[i]->GetAllChildActors(childActors);
				for (AActor *childActor : childActors)
				{
					childActor->Destroy();
				}
			}

			if (actors[i]->IsA<AEnvironmentActor>())
			{
				if (actors[i]->IsA<ACeilingAmbiantLight>() ||
					actors[i]->IsA<ACeilingStalagtite>() ||
					actors[i]->IsA<AMudFloor>())
					continue;

				actors[i]->Destroy();
				continue;
			}
			if (actors[i]->IsA<ACharacter>())
			{
				actors[i]->Destroy();
				continue;
			}
			if (actors[i]->IsA<AWorldGridItemActor>())
			{
				actors[i]->Destroy();
				continue;
			}
		}
	}
}
#pragma optimize("", on)
void AWorldGenerationActors::BeginDestroy()
{
	if (BaseRunnable)
	{
		BaseRunnable->KillThread();
		BaseRunnable->EnsureCompletion();
	}

	Super::BeginDestroy();
}

void AWorldGenerationActors::RunWinLossRunnable()
{
	USvUtilities::AttemptToStartWinLossChecker(GetWorld());
}

void AWorldGenerationActors::DestroyAllDebugActors()
{
	auto actors = GetWorld()->GetCurrentLevel()->Actors;
	auto totalActors = actors.Num() - 1;
	for (int i = totalActors; i > 0; i--)
	{
		if (actors[i])
		{
			if (actors[i]->IsA<AWorldGridItemActor>())
			{
				actors[i]->Destroy();
				continue;
			}
		}
	}
}

void AWorldGenerationActors::GenerateFog()
{
	AFogManager *fogManager = nullptr;
	auto actors = GetWorld()->GetCurrentLevel()->Actors;
	auto totalActors = actors.Num() - 1;
	for (int i = totalActors; i > 0; i--)
	{
		if (actors[i] && actors[i]->IsA<AFogManager>())
		{
			fogManager = Cast<AFogManager>(actors[i]);
			break;
		}
	}

	auto elevation = 1;
	auto maxX = 40;
	auto maxY = 40;

	TArray<FVector> fogLocations;

	for (int j = 0; j <= elevation; j++)
		for (int i = 0; i <= maxX; i++)
			for (int x = 0; x <= maxY; x++)
				fogLocations.Emplace(FVector(i * 100, x * 100, j * 200));

	fogManager->AddComponentsAtLocation(fogLocations);
}
