// Fill out your copyright notice in the Description page of Project Settings.


#include "SvUtilities.h"
#include "../GameModes/GameplayMode.h"
#include "../Interfaces/Gameplay.h"
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
	adjacentTiles.Emplace(location + FVector(gridGape, 0, 0));
	adjacentTiles.Emplace(location + FVector(-gridGape, 0, 0));
	adjacentTiles.Emplace(location + FVector(0, gridGape, 0));
	adjacentTiles.Emplace(location + FVector(0, -gridGape, 0));
}