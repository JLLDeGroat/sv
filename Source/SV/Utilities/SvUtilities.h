// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "../Enums/EWorldEnums.h"
#include "../Enums/EEquipmentEnums.h"
#include "SvUtilities.generated.h"
/**
 *
 */
class IGameplay;
class UNiagaraSystem;
class UCharacterManager;
class UObjectivesManager;
class UWinLossManager;
class USvGameInstance;
class UTexture2D;
class UTextureRenderTarget2D;

struct FCurrentGameData;

UCLASS()
class SV_API USvUtilities : public UObject
{
	GENERATED_BODY()
public:
	static ECollisionChannel GetFloorTargetChannel();
	static ECollisionChannel GetClickableChannel();
	static ECollisionChannel GetClickableEnvironmentChannel();
	static ECollisionChannel GetEnvironmentChannel();
	static ECollisionChannel GetBulletCollisionChannel();

	static ECollisionChannel GetBulletCollisionObjectChannel();

	static ECollisionChannel GetWorldSelectChannel();

	static ECollisionChannel GetTriggerableChannel();

	static int FormatLocation(float val);

	static UStaticMesh* GetStaticMesh(FString reference);
	static USkeletalMesh* GetSkeletonMesh(FString reference);
	static UNiagaraSystem* GetNiagaraSystem(FString reference);
	static UMaterial* GetMaterial(FString reference);
	static UClass* GetClass(FString reference);
	static UTexture2D* GetTexture(FString reference);
	static UTextureRenderTarget2D* GetRenderTarget2D(int targetNumber);

	static float GetGridGape();

	static void GetAdjacentGridTiles(FVector location, TArray<FVector>& adjacentTiles);
	static bool AreGridLocationsAdjacent(FVector loc1, FVector loc2);
	static int GetTileElevation(FVector loc);

	static TArray<FVector> OrderByClosestTo(FVector location, TArray<FVector> locationArray);

	static TScriptInterface<IGameplay> GetGameMode(UWorld* world);
	static UCharacterManager* GetGameModeCharacterManager(UWorld* world);
	static UObjectivesManager* GetGameModeObjectiveManager(UWorld* world);

	static USvGameInstance* GetGameInstance(UWorld* world);
	static FCurrentGameData* GetCurrentGameData(UWorld* world);

	static int GetWorldMapGridMultiplier();

	static TArray<FVector> RandomizeList(TArray<FVector> list, FRandomStream stream);
	static TArray<FVector> RandomizeList(TArray<FVector> list);


	static FVector DetermineAccuracyInidicatorScale(FVector source, FVector target, float accuracy, float accuracyDecay, float baseAccuracy, float distanceModifier = 1.0f);

	static void AttemptToStartWinLossChecker(UWorld* world);

	static AActor* AttemptToGetCurrentSelectedActor(UWorld* world);

	static void AttemptToStartStatUpdater(AActor* statOwner, EStatisticType statType, float value = 0.0f);

	static FString GetSocketNameFromAttachment(EAttachType attachmentType);

	static UMaterial* GetBulletHoleMaterial();

private:

	static bool IsInBounds(FVector location);
};
