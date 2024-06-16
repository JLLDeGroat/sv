// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerationManager.h"

void ULevelGenerationManager::SetGridItems(TArray<FVector> arr) {
	GridItems = arr;
}
void ULevelGenerationManager::SetSpawnLocations(TArray<FVector> arr) {
	SpawnLocations = arr;
}
void ULevelGenerationManager::SetEndLocations(TArray<FVector> arr) {
	EndLocations = arr;
}

TArray<FVector> ULevelGenerationManager::GetGridItems() {
	return GridItems;
}
TArray<FVector> ULevelGenerationManager::GetSpawnLocations() {
	return SpawnLocations;
}
TArray<FVector> ULevelGenerationManager::GetEndLocations() {
	return EndLocations;
}