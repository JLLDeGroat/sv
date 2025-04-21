// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Containers/Map.h"
#include "DirectorManager.generated.h"

USTRUCT()
struct FWallSpawnLoc
{
	GENERATED_BODY()
public:
	FWallSpawnLoc() : Wall(FVector::ZeroVector), End(FVector::ZeroVector)
	{
	}
	FWallSpawnLoc(FVector wall, FVector end) : Wall(wall), End(end)
	{
	}

	FVector GetWall() { return Wall; }
	FVector GetEnd() { return End; }

protected:
private:
	UPROPERTY()
	FVector Wall;
	UPROPERTY()
	FVector End;
};

USTRUCT()
struct FDirectorCrewStats
{
	GENERATED_BODY()
public:
	FDirectorCrewStats() : StatOwner(nullptr), Movement(0.0f), Damage(0.0f), Kills(0), Turn(0), Noise(0)
	{
	}
	FDirectorCrewStats(AActor *actor, int turn) : StatOwner(actor), Movement(0.0f), Damage(0.0f), Kills(0), Turn(turn), Noise(0)
	{
	}

	int GetTurn() const { return Turn; }
	AActor *GetStatOwner() const { return StatOwner; }
	float GetDamage() const { return Damage; }
	float GetMovement() const { return Movement; }
	int GetKills() const { return Kills; }
	float GetNoise() const { return Noise; }

	void AddKills(int amount) { Kills += amount; }
	void AddDamage(float damage) { Damage += damage; }
	void AddMovement(float amount) { Movement += amount; }
	void AddNoise(float amount) { Noise += amount; }

protected:
private:
	UPROPERTY()
	AActor *StatOwner;
	UPROPERTY()
	float Movement;
	UPROPERTY()
	float Damage;
	UPROPERTY()
	int Kills;
	UPROPERTY()
	int Turn;
	UPROPERTY()
	float Noise;
};

USTRUCT()
struct FDirectorGenericStats
{
	GENERATED_BODY()
public:
	FDirectorGenericStats()
	{
	}

protected:
private:
	UPROPERTY()
	int Deaths;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UDirectorManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UDirectorManager(const FObjectInitializer &ObjectInitializer);

	TArray<FDirectorCrewStats *> GetAllCrewStats();
	FDirectorCrewStats *GetCrewStats(AActor *actor);
	bool TryAddCrewStat(AActor *actor);

	void MapAvailableSpawns(TArray<TPair<FVector, FVector>> availableMapItems);
	void SetAvailableFloorSpawns(TArray<FVector> availableFloorSpawns);

	TArray<FVector> GetFloorSpawnLocations();
	TArray<FWallSpawnLoc> GetWallSpawnLocations();

protected:
private:
	int GetTurn();

	UPROPERTY()
	TArray<FDirectorCrewStats> DirectorCrewStats;
	UPROPERTY()
	FDirectorGenericStats DirectorGenericStats;

	UPROPERTY()
	TArray<FVector> FloorSpawnLocations;
	UPROPERTY()
	TArray<FWallSpawnLoc> WallSpawnLocations;
};
