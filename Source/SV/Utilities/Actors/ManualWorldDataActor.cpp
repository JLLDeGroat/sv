// Fill out your copyright notice in the Description page of Project Settings.


#include "ManualWorldDataActor.h"
#include "../../Instance/SvGameInstance.h"
#include "../../Instance/Managers/CurrentGameDataManager.h"
#include "../../Instance/Managers/MissionDetailsManager.h"
#include "../../GameModes/GameplayMode.h"
#include "../../GameModes/Managers/ObjectivesManager.h"

#include "../../Runnables/ClassicGameMapGenerationRunnable.h"
#include "../../Utilities/SvUtilities.h"

// Sets default values
AManualWorldDataActor::AManualWorldDataActor()
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
void AManualWorldDataActor::BeginPlay()
{
	Super::BeginPlay();

	GenerateWorldLocationData();
	GenerateWorldLocationMissionsData();
}
void AManualWorldDataActor::GenerateWorldLocationData() {
	auto instance = USvUtilities::GetGameInstance(GetWorld());

	auto currentgameDataManager = instance->GetCurrentGameDataManager();
	if (!currentgameDataManager || !currentgameDataManager->GetCurrentGameData())
		return UDebugMessages::LogError(this, "could not get current game data manager");

	auto currentGameData = currentgameDataManager->GetCurrentGameData();

	TArray<FVector2D> ChosenPrimaryRoute;
	ChosenPrimaryRoute.Emplace(FVector2D(1, 2));
	ChosenPrimaryRoute.Emplace(FVector2D(1, 3));
	ChosenPrimaryRoute.Emplace(FVector2D(1, 4));
	ChosenPrimaryRoute.Emplace(FVector2D(1, 5));
	ChosenPrimaryRoute.Emplace(FVector2D(1, 6));
	ChosenPrimaryRoute.Emplace(FVector2D(1, 7));
	ChosenPrimaryRoute.Emplace(FVector2D(1, 8));
	ChosenPrimaryRoute.Emplace(FVector2D(1, 9));
	ChosenPrimaryRoute.Emplace(FVector2D(2, 9));
	ChosenPrimaryRoute.Emplace(FVector2D(3, 9));
	ChosenPrimaryRoute.Emplace(FVector2D(4, 9));

	TArray<FVector2D>Offshoots;
	Offshoots.Emplace(FVector2D(2, 2));
	Offshoots.Emplace(FVector2D(3, 2));

	auto worldManager = currentGameData->GetWorldData();
	for (int i = 0; i < ChosenPrimaryRoute.Num(); i++) {
		auto locationId = worldManager->AddWorldLocationData(ChosenPrimaryRoute[i]);
		if (i == 0) {
			auto thisLocation = worldManager->GetWorldLocationData(locationId);
			thisLocation->SetIsCurrent(true);
		}
	}

	for (int i = 0; i < Offshoots.Num(); i++) {
		auto locationId = worldManager->AddWorldLocationData(Offshoots[i]);
		auto thisLocation = worldManager->GetWorldLocationData(locationId);
		thisLocation->SetIsOffshoot(true);
	}
}

void AManualWorldDataActor::GenerateWorldLocationMissionsData() {
	auto instance = USvUtilities::GetGameInstance(GetWorld());

	auto currentgameDataManager = instance->GetCurrentGameDataManager();
	if (!currentgameDataManager || !currentgameDataManager->GetCurrentGameData())
		return UDebugMessages::LogError(this, "could not get current game data manager");

	auto currentGameData = currentgameDataManager->GetCurrentGameData();

	auto worldData = currentGameData->GetWorldData();

	auto missionDetailsManager = instance->GetMissionDetailsManager();
	auto worldDataLocations = worldData->GetWorldLocationData();

	for (int i = 0; i < worldDataLocations.Num(); i++) {
		EMissionType missionType = (EMissionType)FMath::RandRange(1, 4);
		auto thisMissionName = missionDetailsManager->GenerateMissionName();
		auto thisMissionDesc = missionDetailsManager->GetMissionTypeDescription(missionType);

		auto mDetails = worldDataLocations[i]->GetMissionDetails();
		mDetails->SetMissionType(missionType);
		mDetails->SetName(thisMissionName);
		mDetails->SetDescription(thisMissionDesc->GetDescription());
		mDetails->SetIsValidMission(true);
		mDetails->SetFluffText(missionDetailsManager->GenerateFluffText(missionType));
		mDetails->SetMainObjective(missionDetailsManager->GenerateMainObjective(missionType));
		mDetails->SetTurnLimit(999);

		if (missionType == EMissionType::MT_Survive)
			mDetails->SetTurnLimit(12);

		if (worldDataLocations[i]->GetIsCurrent()) {

			if (MissionType != EMissionType::INVALID) {
				UDebugMessages::LogDisplay(this, "Manual set missionType");
				//manual set
				mDetails->SetMissionType(MissionType);
				mDetails->SetName(thisMissionName);
				mDetails->SetDescription(thisMissionDesc->GetDescription());
				mDetails->SetIsValidMission(true);
				mDetails->SetFluffText(missionDetailsManager->GenerateFluffText(MissionType));
				mDetails->SetMainObjective(missionDetailsManager->GenerateMainObjective(MissionType));
				mDetails->SetTurnLimit(999);

				if (MissionType == EMissionType::MT_Survive)
					mDetails->SetTurnLimit(12);
			}

			currentGameData->StartNewMission(mDetails->GetName(), mDetails->GetMissionType(), mDetails);
			auto gameMode = GetWorld()->GetAuthGameMode<AGameplayMode>();
			if (!gameMode)
				return UDebugMessages::LogError(this, "failed to get auth game mode");

			auto objectivesManager = gameMode->GetComponentByClass<UObjectivesManager>();
			if (!objectivesManager)
				return UDebugMessages::LogError(this, "failed to get objectives manager");

			objectivesManager->SetupMainObjective();
		}
	}
}