// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "FCrewMemberStats.generated.h"

USTRUCT()
struct SV_API FCrewMemberStats
{
	GENERATED_BODY()
public:
	FCrewMemberStats();

	int GetTotalDamage() const { return TotalDamage; }
	int GetKills() const { return Kills; }
	int GetMissionsCompleted() const { return MissionsCompleted; }
	int GetMovement() const { return Movement; }

	void AddToDamage(int value) { TotalDamage += value; }
	void AddToKills(int value) { Kills += value; }
	void AddToMissionsCompleted(int value) { MissionsCompleted += value; }
	void AddToMovement(int value) { Movement += value; }

protected:
	UPROPERTY()
	int TotalDamage;
	UPROPERTY()
	int Kills;
	UPROPERTY()
	int Movement;
	UPROPERTY()
	int MissionsCompleted;
};
