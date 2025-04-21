// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../Enums/EWorldEnums.h"
#include "../../../Enums/EEquipmentEnums.h"
#include "FMissionStats.h"
#include "FMissionDetails.generated.h"

USTRUCT()
struct SV_API FMissionDetails
{
	GENERATED_BODY()
public:
	FMissionDetails() : Name(""), Description(""), MissionType(EMissionType::INVALID), MissionTypeDescription(""),
						FluffText(""), MainObjective(""), Turn(1), bValidMission(false), TurnLimit(0), bIsCompleted(false),
						bExtractedIntel(false), MissionStats(FMissionStats()) {}
	FMissionDetails(FString name, FString desc, EMissionType type, FString missionTypeDescription)
		: Name(name), Description(desc), MissionType(type), MissionTypeDescription(missionTypeDescription), FluffText(""),
		  MainObjective(""), Turn(1), bValidMission(true), TurnLimit(0), bIsCompleted(false),
		  bExtractedIntel(false), MissionStats(FMissionStats()) {}

	FString GetName();
	FString GetDescription();
	FString GetMissionTypeDescription();
	EMissionType GetMissionType();
	bool GetIsValidMission();
	bool GetIsCompleted();
	FString GetFluffText();
	FString GetMainObjective();
	int GetTurn();
	int GetTurnLimit();
	TMap<EResourceType, int> GetResourceExtracted();
	bool GetExtractedIntel();

	void SetName(FString name);
	void SetDescription(FString desc);
	void SetMissionType(EMissionType missionType);
	void SetMissionTypeDescription(FString missionTypeDesc);
	void SetIsValidMission(bool val);
	void SetIsCompleted(bool val);
	void SetFluffText(FString txt);
	void SetMainObjective(FString txt);
	void AddToTurnCounter();
	void SetTurnLimit(int turnLimit);

	void AddToResourceExtracted(EResourceType rType, int amount);
	void SetHasExtractedIntel();

	FMissionStats *GetMissionStats();

protected:
	UPROPERTY()
	FString Name;
	UPROPERTY()
	FString Description;
	UPROPERTY()
	EMissionType MissionType;
	UPROPERTY()
	FString MissionTypeDescription;
	UPROPERTY()
	FString FluffText;
	UPROPERTY()
	FString MainObjective;
	UPROPERTY()
	int Turn;
	UPROPERTY()
	bool bValidMission;
	UPROPERTY()
	int TurnLimit;
	UPROPERTY()
	bool bIsCompleted;

	UPROPERTY()
	TMap<EResourceType, int> ResourceExtracted;
	UPROPERTY()
	bool bExtractedIntel;
	UPROPERTY()
	FMissionStats MissionStats;
};
