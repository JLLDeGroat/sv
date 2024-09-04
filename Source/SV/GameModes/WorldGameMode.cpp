// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGameMode.h"
#include "../Player/WorldPawn.h"
#include "../Player/WorldPlayerController.h"
#include "../Hud/WorldHud.h"
#include "../Instance/Managers/RouteDataManager.h"
#include "../Utilities/SvUtilities.h"
#include "../Instance/SvGameInstance.h"
#include "../Instance/Managers/CurrentGameDataManager.h"
#include "../World/WorldGridItemActor.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "WorldManagers/WorldMapWallManager.h"
#include "../World/DirectionOptionActor.h"
#include "WorldManagers/WorldDirectionManager.h"
#include "../Utilities/GridUtilities.h"

AWorldGameMode::AWorldGameMode() {
	PlayerControllerClass = AWorldPlayerController::StaticClass();
	DefaultPawnClass = AWorldPawn::StaticClass();
	HUDClass = AWorldHud::StaticClass();

	RouteWallManager = CreateDefaultSubobject<UWorldMapWallManager>(TEXT("WallManager"));
	DirectionManager = CreateDefaultSubobject<UWorldDirectionManager>(TEXT("DirectionManager"));
}

void AWorldGameMode::BeginPlay() {
	Super::BeginPlay();

	auto instance = USvUtilities::GetGameInstance(GetWorld());
	if (!instance->GetCurrentGameDataManager() || !instance->GetCurrentGameDataManager()->GetCurrentGameData())
		return UDebugMessages::LogError(this, "failed to get current game data manager");

	auto currentGameData = instance->GetCurrentGameDataManager()->GetCurrentGameData();
	auto worldData = currentGameData->GetWorldData();

	if (!RouteWallManager) return UDebugMessages::LogError(this, "failed to get route wall manager");

	TArray<FVector2D> fullRoute = worldData->GetTotalRoute();
	RouteWallManager->SetFullRouteList(fullRoute);
	DirectionManager->SetFullRouteList(fullRoute);
	RouteWallManager->GenerateWalls();

	auto currentStep = worldData->GetCurrentLocation();

	auto allActors = GetWorld()->GetCurrentLevel()->Actors;
	for (int i = 0; i < allActors.Num(); i++) {
		if (allActors[i] && allActors[i]->IsA<APawn>()) {
			auto firstStep3DLocation = UGridUtilities::GetRouteLocationAs3DLoc(currentStep->GetLocation(), true);
			allActors[i]->SetActorLocation(firstStep3DLocation + FVector(0, 0, 68));
			break;
		}
	}

	auto currentMission = currentGameData->GetCurrentMission();
	if (currentMission && currentMission->GetMissionDetails()) {
		auto missionDetails = currentMission->GetMissionDetails();
		if (missionDetails->GetIsCompleted()) {
			auto extractedResources = missionDetails->GetResourceExtracted();
			auto currentResources = currentGameData->GetResourceData();

			for (const TPair<EResourceType, int>& pair : extractedResources)
			{
				auto res = currentResources->GetResource(pair.Key);
				res->AddToAmount(pair.Value);
				UDebugMessages::LogDisplay(this, "added (" + FString::SanitizeFloat(pair.Value) + ") resource of " + EEquipmentEnums::GetResourceTypeAsString(EResourceType::RT_Currency));
			}
		}
	}

	DirectionManager->GenerateDirections(currentStep->GetLocation());
}