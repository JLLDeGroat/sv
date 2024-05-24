// Fill out your copyright notice in the Description page of Project Settings.


#include "SvUtilities.h"
#include "../GameModes/GameplayMode.h"
#include "../Interfaces/Gameplay.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "NiagaraSystem.h"

ECollisionChannel USvUtilities::GetFloorTargetChannel() {
	return ECollisionChannel::ECC_GameTraceChannel1;
}
ECollisionChannel USvUtilities::GetClickableChannel() {
	return ECollisionChannel::ECC_GameTraceChannel2;
}
ECollisionChannel USvUtilities::GetEnvironmentChannel() {
	return ECollisionChannel::ECC_GameTraceChannel3;
}
ECollisionChannel USvUtilities::GetBulletCollisionChannel() {
	return ECollisionChannel::ECC_GameTraceChannel4;
}

ECollisionChannel USvUtilities::GetBulletCollisionObjectChannel() {
	return ECC_GameTraceChannel5;
}

int USvUtilities::FormatLocation(float val) {
	int value = val;

	if (value == 0) return 0;

	auto left = (int)val % 50;

	auto addition = left > 25 ?
		-(50 - left) :
		left;

	return value - addition;
}

UStaticMesh* USvUtilities::GetStaticMesh(FString reference) {
	return Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *reference, NULL, LOAD_None, NULL));
}

USkeletalMesh* USvUtilities::GetSkeletonMesh(FString reference) {
	return Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), NULL, *reference, NULL, LOAD_None, NULL));
}

UNiagaraSystem* USvUtilities::GetNiagaraSystem(FString reference) {
	return Cast<UNiagaraSystem>(StaticLoadObject(UNiagaraSystem::StaticClass(), NULL, *reference, NULL, LOAD_None, NULL));
}

UMaterial* USvUtilities::GetMaterial(FString reference) {
	return Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, *reference, NULL, LOAD_None, NULL));
}

UClass* USvUtilities::GetClass(FString reference) {
	return Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *reference, NULL, LOAD_None, NULL));
}

float USvUtilities::GetGridGape() { return 100.0f; }

TScriptInterface<IGameplay> USvUtilities::GetGameMode(UWorld* world) {
	return Cast<AGameplayMode>(world->GetAuthGameMode());
}

void USvUtilities::GetAdjacentGridTiles(FVector location, TArray<FVector>& adjacentTiles) {
	auto gridGape = GetGridGape();

	if (IsInBounds(location + FVector(gridGape, 0, 0)))
		adjacentTiles.Emplace(location + FVector(gridGape, 0, 0));

	if (IsInBounds(location + FVector(-gridGape, 0, 0)))
		adjacentTiles.Emplace(location + FVector(-gridGape, 0, 0));
	
	if (IsInBounds(location + FVector(0, gridGape, 0)))
		adjacentTiles.Emplace(location + FVector(0, gridGape, 0));
	
	if (IsInBounds(location + FVector(0, -gridGape, 0)))
		adjacentTiles.Emplace(location + FVector(0, -gridGape, 0));
}

bool USvUtilities::IsInBounds(FVector location) {
	if (location.X > 3000 || location.X < -3000 ||
		location.Y > 3000 || location.Y < -3000)
		return false;

	return true;
}

bool USvUtilities::AreGridLocationsAdjacent(FVector loc1, FVector loc2) {

	if (GetTileElevation(loc1) == GetTileElevation(loc2)) {

		TArray<FVector> adjacentTiles;
		GetAdjacentGridTiles(loc1, adjacentTiles);

		for (int i = 0; i < adjacentTiles.Num(); i++) {
			if (adjacentTiles[i].X == loc2.X && 
				adjacentTiles[i].Y == loc2.Y) 
				return true;
		}
	}
	return false;
}

TArray<FVector> USvUtilities::OrderByClosestTo(FVector location, TArray<FVector> unorderedLocations) {
	TArray<FVector> adjacentTiles;

	if (location != FVector::ZeroVector) {
		adjacentTiles.Empty();
		while (unorderedLocations.Num() > 0) {
			float minDistance = -1;
			FVector loc = FVector::ZeroVector;
			auto index = -1;
			for (int i = 0; i < unorderedLocations.Num(); i++) {
				auto distance = FVector::Dist(unorderedLocations[i], location);
				//UDebugMessages::LogDisplay(this, "distance between " + location.ToString() + " and " + unorderedLocations[i].ToString() + " was " + FString::SanitizeFloat(distance, 2));
				if (loc == FVector::ZeroVector || distance < minDistance) {
					minDistance = distance;
					loc = unorderedLocations[i];
					index = i;
				}
			}

			adjacentTiles.Emplace(unorderedLocations[index]);
			unorderedLocations.RemoveAt(index);
		}
	}

	return adjacentTiles;
}

int USvUtilities::GetTileElevation(FVector loc) {
	int val = 1;
	if (loc.Z > 200) val = 2;
	else if (loc.Z > 400) val = 3;
	else if (loc.Z > 600) val = 4;
	else if (loc.Z > 800) val = 5;

	return val;
}