#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Enums/EWorldEnums.h"
#include "FMissionNameData.generated.h"




USTRUCT()
struct FMissionTitles {
	GENERATED_BODY()
public:
	FMissionTitles() {

	}

	TArray<FString> GetMissionAdjectives() { return MissionAdjectives; }
	TArray<FString> GetMissionNouns() { return MissionNouns; }

protected:

	UPROPERTY() TArray<FString> MissionAdjectives;
	UPROPERTY() TArray<FString> MissionNouns;
};
USTRUCT()
struct FMissionDescriptions {
	GENERATED_BODY()
public:
	FMissionDescriptions() {
		MissionType = EMissionType::INVALID;
		Description = "";
		MissionName = "";
	}

	EMissionType GetMissionType() { return MissionType; }
	FString GetDescription() { return Description; }
	FString GetMissionName() { return MissionName; }

protected:
	UPROPERTY() FString Description;
	UPROPERTY() EMissionType MissionType;
	UPROPERTY() FString MissionName;
};

/**
 *
 */
USTRUCT()
struct FMissionNameData
{
	GENERATED_BODY()
public:
	FMissionNameData() {
		MissionTitles = FMissionTitles();
	}

	FMissionTitles* GetMissionTitles() {
		return &MissionTitles;
	}

	FMissionDescriptions* GetDescription(EMissionType missionType) {
		for (int i = 0; i < MissionDescriptions.Num(); i++)
			if (MissionDescriptions[i].GetMissionType() == missionType)
				return &MissionDescriptions[i];

		return nullptr;
	}

protected:
	UPROPERTY() FMissionTitles MissionTitles;
	UPROPERTY() TArray<FMissionDescriptions> MissionDescriptions;
};
