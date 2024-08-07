// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../Enums/EWorldEnums.h"
#include "FMissionDetails.generated.h"


USTRUCT()
struct SV_API FMissionDetails
{
	GENERATED_BODY()
public:
	FMissionDetails();
	FMissionDetails(FString name, FString desc, EMissionType type, FString missionTypeDescription);

	FString GetName();
	FString GetDescription();
	FString GetMissionTypeDescription();
	EMissionType GetMissionType();
	bool GetIsValidMission();
	bool GetIsCompleted();

	void SetName(FString name);
	void SetDescription(FString desc);
	void SetMissionType(EMissionType missionType);
	void SetIsValidMission(bool val);
	void SetIsCompleted(bool val);

protected:

	UPROPERTY() FString Name;
	UPROPERTY() FString Description;
	UPROPERTY() EMissionType MissionType;
	UPROPERTY() FString MissionTypeDescription;
	UPROPERTY() bool bValidMission;

	UPROPERTY() bool bIsCompleted;

};
