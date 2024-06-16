// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassicGameMapGenerationRunnable.h"
#include "../Hud/MenuHuds/Components/NewGameOptionsWidget.h"
#include "../Instance/SvGameInstance.h"
#include "../Instance/Managers/RouteDataManager.h"
#include "../Instance/Managers/CrewDataManager.h"
#include "../Data/FGameData.h"
#include "../Utilities/SvUtilities.h"
#include "../Utilities/RunnableUtilities.h"

#pragma optimize("", off)
void UClassicGameMapGenerationRunnable::ActivateThread() {
	Super::ActivateThread();

	Start = FVector2D(RandomStream.RandRange(1, 2), RandomStream.RandRange(1, 8));
	End = FVector2D(RandomStream.RandRange(11, 12), RandomStream.RandRange(1, 8));
	MaxOffshoots = 16;

	auto widget = GameModeWidget;

	FindValidRoutes();
	FindValidOffshoots();

	auto instance = USvUtilities::GetGameInstance(GetWorld());
	auto routeManager = instance->GetRouteDataManager();

	if (!routeManager) UDebugMessages::LogError(this, "failed to get route data manager");
	routeManager->SetCurrentRoute(ChosenPrimaryRoute);
	routeManager->SetCurrentOffshoots(Offshoots);
	routeManager->SetCurrentLocationOnRoute(ChosenPrimaryRoute[0]);

	GenerateCrewMembers();

	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([widget] {
		widget->OnGenCompleted();
		}, TStatId(), nullptr, ENamedThreads::GameThread);
}

UClassicGameMapGenerationRunnable* UClassicGameMapGenerationRunnable::InsertVariables(UNewGameOptionsWidget* widget, FMapGridData* mapGrid) {
	GameModeWidget = widget;
	CurrentMapGrid = *mapGrid;
	ChosenPrimaryRoute.Empty();
	return this;
}

void UClassicGameMapGenerationRunnable::FindValidRoutes() {
	TArray<FVector2D> route;
	route.Emplace(Start);
	FindValidRoutesRecursive(route);

	auto widget = GameModeWidget;
	auto finalRoute = ChosenPrimaryRoute;
}
void UClassicGameMapGenerationRunnable::FindValidRoutesRecursive(TArray<FVector2D> currentRoute) {

	if (currentRoute.Num() > 26 ||
		!ChosenPrimaryRoute.IsEmpty())
		return;

	auto latestRouteItem = currentRoute[currentRoute.Num() - 1];
	auto validAdjacents = URunnableUtilities::GetPassableAdjacentGridItems(&CurrentMapGrid, latestRouteItem);

	for (int i = 0; i < validAdjacents.Num(); i++) {
		if (!ChosenPrimaryRoute.IsEmpty()) return;
		if (AlreadyInRoute(currentRoute, validAdjacents[i])) {
			continue;
		}

		auto newRoute = currentRoute;
		newRoute.Emplace(validAdjacents[i]);

		if (IsAdjacentToNonPreviousLocations(currentRoute, validAdjacents[i]) || !IsPossibleToReachEnd(newRoute))
			continue;

		if (End == validAdjacents[i]) {
			if (ChosenPrimaryRoute.IsEmpty())
				ChosenPrimaryRoute = newRoute;
		}
		else {
			FindValidRoutesRecursive(newRoute);
		}
	}
}
bool UClassicGameMapGenerationRunnable::AlreadyInRoute(TArray<FVector2D> route, FVector2D loc) {
	for (int i = 0; i < route.Num(); i++)
		if (route[i] == loc) return true;

	return false;
}
bool UClassicGameMapGenerationRunnable::IsPossibleToReachEnd(TArray<FVector2D> current) {
	auto last = current[current.Num() - 1];
	auto movementLeft = 25 - current.Num();

	auto xmovement = last.X - End.X;
	if (xmovement < 0) xmovement *= -1;

	auto ymovement = last.Y - End.Y;
	if (ymovement < 0) ymovement *= -1;

	if (ymovement + xmovement <= movementLeft) {
		return true;
	}
	else return false;
}
bool UClassicGameMapGenerationRunnable::IsAdjacentToNonPreviousLocations(TArray<FVector2D> previousRoute, FVector2D newLoc) {
	if (previousRoute.Num() == 1) return false;

	for (int i = 0; i < previousRoute.Num() - 1; i++) {
		auto left = FVector2D(previousRoute[i].X - 1, previousRoute[i].Y);
		auto right = FVector2D(previousRoute[i].X + 1, previousRoute[i].Y);
		auto up = FVector2D(previousRoute[i].X, previousRoute[i].Y - 1);
		auto down = FVector2D(previousRoute[i].X, previousRoute[i].Y + 1);

		if (newLoc == left || newLoc == right || newLoc == up || newLoc == down)
			return true;
	}

	return false;
}


void UClassicGameMapGenerationRunnable::FindValidOffshoots() {
	while (Offshoots.Num() < MaxOffshoots)
		for (int i = 1; i < ChosenPrimaryRoute.Num() - 1; i++) {
			auto newArr = TArray<FVector2D>();
			newArr.Emplace(ChosenPrimaryRoute[i]);
			FindValidOffshootsRecursive(newArr, true);
		}

	auto widget = GameModeWidget;
	auto finalOffshoots = Offshoots;
	UDebugMessages::LogDisplay(this, "Finished Offshoots");
	UDebugMessages::LogDisplay(this, "found numbers " + FString::SanitizeFloat(finalOffshoots.Num()));
	for (int i = 0; i < finalOffshoots.Num(); i++) {
		UDebugMessages::LogDisplay(this, finalOffshoots[i].ToString());
	}
}

void UClassicGameMapGenerationRunnable::FindValidOffshootsRecursive(TArray<FVector2D> currentOffshoot, bool isInitial) {
	if (Offshoots.Num() == MaxOffshoots)
		return;


	auto lastRoute = currentOffshoot[currentOffshoot.Num() - 1];
	auto adjacentGridItems = URunnableUtilities::GetPassableAdjacentGridItems(&CurrentMapGrid, lastRoute);
	
	if (isInitial)
		currentOffshoot.RemoveAt(0);

	for (int i = 0; i < adjacentGridItems.Num(); i++) {

		if (isInitial && IsAdjacentToOtherOffshoots(adjacentGridItems[i])) {
			continue;
		}

		if (IsPartOfMainRoute(adjacentGridItems[i])) {
			continue;
		}

		if (!ShouldGenerateOffShoot()) {
			continue;
		}

		if (HasReachedMaxOffshoots()) {
			continue;
		}

		if (AlreadyAnOffshoot(adjacentGridItems[i])) {
			continue;
		}

		auto newRoute = currentOffshoot;
		newRoute.Emplace(adjacentGridItems[i]);
		Offshoots.Emplace(adjacentGridItems[i]);
		FindValidOffshootsRecursive(newRoute);
	}
}

bool UClassicGameMapGenerationRunnable::ShouldGenerateOffShoot(int degradation, int incrememtation) {
	if (BaseChance > RandomStream.RandRange(1, 100)) {
		BaseChance -= degradation;
		return true;
	}
	BaseChance += incrememtation;
	return false;
}

bool UClassicGameMapGenerationRunnable::IsPartOfMainRoute(FVector2D arr) {
	for (int i = 0; i < ChosenPrimaryRoute.Num(); i++)
		if (arr == ChosenPrimaryRoute[i])
			return true;

	return false;
}

bool UClassicGameMapGenerationRunnable::HasReachedMaxOffshoots() {
	return  Offshoots.Num() >= MaxOffshoots;
}

bool UClassicGameMapGenerationRunnable::AlreadyAnOffshoot(FVector2D vector) {
	for (int i = 0; i < Offshoots.Num(); i++)
		if (Offshoots[i] == vector) return true;

	return false;
}

bool UClassicGameMapGenerationRunnable::IsAdjacentToOtherOffshoots(FVector2D vector) {
	auto left = FVector2D(vector.X - 1, vector.Y);
	auto right = FVector2D(vector.X + 1, vector.Y);
	auto up = FVector2D(vector.X, vector.Y - 1);
	auto down = FVector2D(vector.X, vector.Y + 1);

	for (int i = 0; i < Offshoots.Num(); i++)
		if (Offshoots[i] == left ||
			Offshoots[i] == right ||
			Offshoots[i] == up ||
			Offshoots[i] == down) return true;

	return false;
}

void UClassicGameMapGenerationRunnable::GenerateCrewMembers(int amount) {
	auto instance = USvUtilities::GetGameInstance(GetWorld());
	auto crewData = instance->GetPossibleCrewData();
	auto currentCrewData = instance->GetCrewManager();

	auto allFirstNames = crewData->GetFirstNames();
	auto allLastNames = crewData->GetLastNames();
	auto allBios = crewData->GetBios();
	for (int i = 0; i < amount; i++) {
		FCrewMember newMember = FCrewMember();
		newMember.SetName(allFirstNames[RandomStream.RandRange(1, allFirstNames.Num() - 1)]);
		newMember.SetLastName(allLastNames[RandomStream.RandRange(1, allLastNames.Num() - 1)]);
		newMember.SetBio(allBios[RandomStream.RandRange(1, allBios.Num() - 1)]);
		currentCrewData->AddCrewMember(&newMember);
	}
}
#pragma optimize("", on)