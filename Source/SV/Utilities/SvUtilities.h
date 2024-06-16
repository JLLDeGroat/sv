// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SvUtilities.generated.h"
/**
 *
 */
class IGameplay;
class UNiagaraSystem;
class UCharacterManager;
class USvGameInstance;

UCLASS()
class SV_API USvUtilities : public UObject
{
	GENERATED_BODY()
public:
	static ECollisionChannel GetFloorTargetChannel();
	static ECollisionChannel GetClickableChannel();
	static ECollisionChannel GetEnvironmentChannel();
	static ECollisionChannel GetBulletCollisionChannel();

	static ECollisionChannel GetBulletCollisionObjectChannel();

	static ECollisionChannel GetWorldSelectChannel();

	static int FormatLocation(float val);

	static UStaticMesh* GetStaticMesh(FString reference);
	static USkeletalMesh* GetSkeletonMesh(FString reference);
	static UNiagaraSystem* GetNiagaraSystem(FString reference);
	static UMaterial* GetMaterial(FString reference);
	static UClass* GetClass(FString reference);
	static float GetGridGape();

	static void GetAdjacentGridTiles(FVector location, TArray<FVector>& adjacentTiles);
	static bool AreGridLocationsAdjacent(FVector loc1, FVector loc2);
	static int GetTileElevation(FVector loc);

	static TArray<FVector> OrderByClosestTo(FVector location, TArray<FVector> locationArray);
	
	static TScriptInterface<IGameplay> GetGameMode(UWorld* world);
	static UCharacterManager* GetGameModeCharacterManager(UWorld* world);

	static USvGameInstance* GetGameInstance(UWorld* world);

	static int GetWorldMapGridMultiplier();

private:

	static bool IsInBounds(FVector location);
};
