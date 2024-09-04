#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Enums/EWorldEnums.h"
#include "FMissionNameData.generated.h"

USTRUCT()
struct FMissionObjectives {
	GENERATED_BODY()
public:
	FMissionObjectives() {

	}

	EMissionType GetMissionType() { return MissionType; }
	FString GetText() { return Text; }

protected:

	UPROPERTY() EMissionType MissionType;
	UPROPERTY() FString Text;
};
USTRUCT()
struct FMissionFluffItems {
	GENERATED_BODY()
public:
	FMissionFluffItems() {

	}

	EMissionType GetMissionType() { return MissionType; }
	FString GetFluff() { return Fluff; }

protected:

	UPROPERTY() EMissionType MissionType;
	UPROPERTY() FString Fluff;
};

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

	FString GetRandomFluff(EMissionType missionType) {
		TArray<FString> fluffs;
		for (int i = 0; i < MissionFluffItems.Num(); i++) {
			if (MissionFluffItems[i].GetMissionType() == missionType)
				fluffs.Emplace(MissionFluffItems[i].GetFluff());
		}

		if (fluffs.IsEmpty()) return "NONE";
		else if (fluffs.Num() == 1) return fluffs[0];
		else {
			return fluffs[FMath::RandRange(0, fluffs.Num() - 1)];
		}
	}

	FString GetRandomMainObjective(EMissionType missionType) {
		TArray<FString> fluffs;
		for (int i = 0; i < MissionObjectives.Num(); i++) {
			if (MissionObjectives[i].GetMissionType() == missionType)
				fluffs.Emplace(MissionObjectives[i].GetText());
		}

		if (fluffs.IsEmpty()) return "NONE";
		else if (fluffs.Num() == 1) return fluffs[0];
		else {
			return fluffs[FMath::RandRange(0, fluffs.Num() - 1)];
		}
	}

protected:
	UPROPERTY() FMissionTitles MissionTitles;
	UPROPERTY() TArray<FMissionDescriptions> MissionDescriptions;
	UPROPERTY() TArray<FMissionFluffItems> MissionFluffItems;
	UPROPERTY() TArray<FMissionObjectives> MissionObjectives;
};
