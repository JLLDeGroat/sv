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

	DirectionManager->GenerateDirections(currentStep->GetLocation());
}