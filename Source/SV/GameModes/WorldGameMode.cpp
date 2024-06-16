// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGameMode.h"
#include "../Player/WorldPawn.h"
#include "../Player/WorldPlayerController.h"
#include "../Hud/WorldHud.h"
#include "../Instance/Managers/RouteDataManager.h"
#include "../Utilities/SvUtilities.h"
#include "../Instance/SvGameInstance.h"
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
	auto routeData = instance->GetRouteDataManager();

	auto route = routeData->GetCurrentRoute();
	auto offshoots = routeData->GetCurrentOffshoots();

	int multiplier = USvUtilities::GetWorldMapGridMultiplier();

	for (int i = 0; i < route.Num(); i++) {
		auto actualLocation = FVector(route[i].Y * multiplier, route[i].X * multiplier, 0);
	}

	for (int i = 0; i < offshoots.Num(); i++) {
		auto actualLocation = FVector(offshoots[i].Y * multiplier, offshoots[i].X * multiplier, 0);
	}

	if (!RouteWallManager) return UDebugMessages::LogError(this, "failed to get route wall manager");

	TArray<FVector2D> fullRoute;
	fullRoute.Append(route);
	fullRoute.Append(offshoots);
	RouteWallManager->SetFullRouteList(fullRoute);
	DirectionManager->SetFullRouteList(fullRoute);
	RouteWallManager->GenerateWalls();

	auto currentStep = routeData->GetCurrentLocationOnRoute();

	auto allActors = GetWorld()->GetCurrentLevel()->Actors;
	for (int i = 0; i < allActors.Num(); i++) {
		if (allActors[i] && allActors[i]->IsA<APawn>()) {
			auto firstStep3DLocation = UGridUtilities::GetRouteLocationAs3DLoc(currentStep, true);
			allActors[i]->SetActorLocation(firstStep3DLocation + FVector(0, 0, 68));
			break;
		}
	}

	DirectionManager->GenerateDirections(currentStep);
}