// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SvUtilities.generated.h"
/**
 *
 */
class IGameplay;
class UNiagaraSystem;

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

	static int FormatLocation(float val);

	static UStaticMesh* GetStaticMesh(FString reference);
	static USkeletalMesh* GetSkeletonMesh(FString reference);
	static UNiagaraSystem* GetNiagaraSystem(FString reference);
	static UMaterial* GetMaterial(FString reference);
	static UClass* GetClass(FString reference);
	static float GetGridGape();

	static void GetAdjacentGridTiles(FVector location, TArray<FVector>& adjacentTiles);
	
	static TScriptInterface<IGameplay> GetGameMode(UWorld* world);
};
