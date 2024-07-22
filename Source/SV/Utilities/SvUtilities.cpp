// Fill out your copyright notice in the Description page of Project Settings.


#include "SvUtilities.h"
#include "../GameModes/GameplayMode.h"
#include "../GameModes/Managers/CharacterManager.h"
#include "../Interfaces/Gameplay.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "../Instance/SvGameInstance.h"
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

ECollisionChannel USvUtilities::GetWorldSelectChannel() {
	return ECC_GameTraceChannel6;
}

ECollisionChannel USvUtilities::GetTriggerableChannel() {
	return ECC_GameTraceChannel7;
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

UTexture2D* USvUtilities::GetTexture(FString reference) {
	return Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *reference, NULL, LOAD_None, NULL));
}

float USvUtilities::GetGridGape() { return 100.0f; }

TScriptInterface<IGameplay> USvUtilities::GetGameMode(UWorld* world) {
	return Cast<AGameplayMode>(world->GetAuthGameMode());
}

UCharacterManager* USvUtilities::GetGameModeCharacterManager(UWorld* world) {
	auto gameMode = GetGameMode(world);
	return gameMode->GetCharacterManager();
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
	if (location.X > 10000 || location.X < -1000 ||
		location.Y > 10000 || location.Y < -1000)
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

USvGameInstance* USvUtilities::GetGameInstance(UWorld* world) {
	return world->GetGameInstance<USvGameInstance>();
}

TArray<FVector> USvUtilities::RandomizeList(TArray<FVector> list, FRandomStream stream) {
	TArray<FVector> result = list;
	if (result.Num() > 0)
	{
		int32 LastIndex = result.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = stream.RandRange(i, LastIndex);
			if (i != Index)
				result.Swap(i, Index);
		}
	}
	return result;
}

TArray<FVector> USvUtilities::RandomizeList(TArray<FVector> list) {
	TArray<FVector> result = list;
	if (result.Num() > 0)
	{
		int32 LastIndex = result.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
				result.Swap(i, Index);
		}
	}
	return result;
}

int USvUtilities::GetWorldMapGridMultiplier() {
	return 100;
}

FVector USvUtilities::DetermineAccuracyInidicatorScale(FVector source, FVector target, float accuracy, float accuracyDecay, float baseAccuracy, float distanceModifier) {
	auto gunAccuracy = (FVector::Dist(source, target) * distanceModifier) - accuracy;
	if (gunAccuracy < 1) gunAccuracy = 1;
	gunAccuracy = gunAccuracy * accuracyDecay;
	return FVector(gunAccuracy / 100) + FVector(baseAccuracy);
}