// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../Enums/EEquipmentEnums.h"
#include "../../Enums/EWorldEnums.h"
#include "Mission/FMissionDetails.h"
#include "Stats/FCrewMemberStats.h"
#include "World/FCurrentWorldData.h"
#include "Resource/FResourceData.h"
#include "Tutorials/FTutorialData.h"
#include "../../Characters/DandD/Base/BaseDD.h"
#include "FCurrentGameData.generated.h"

USTRUCT()
struct SV_API FCrew
{
	GENERATED_BODY()
public:
	FCrew()
	{
		LastName = "";
		FirstName = "";
		NickName = "";
		EquippedGun = EGun::INVALID;
		Health = 0;
		MaxHealth = 0;
		Bio = "";
		Stats = FCrewMemberStats();
	}

	FCrew(FString fName, FString lName, FString bio, int health, int maxHealth, FGuid guid = FGuid::NewGuid())
	{
		LastName = fName;
		FirstName = lName;
		NickName = "";
		EquippedGun = EGun::INVALID;
		Health = health;
		MaxHealth = maxHealth;
		Bio = bio;
		Id = guid;
		Stats = FCrewMemberStats();
	}

	FString GetName() const { return FirstName + " " + LastName; }
	FGuid GetId() const { return Id; }
	FCrewMemberStats *GetStats() { return &Stats; }
	void SetHealth(int health) { Health = health; }

	TArray<TSubclassOf<UBaseDD>> GetDirectivesAndDeviations() { return DirectivesAndDeviations; }
	void AddDirectiveAndDeviation(TSubclassOf<UBaseDD> dD) { DirectivesAndDeviations.Emplace(dD); }

protected:
	UPROPERTY()
	FString FirstName;
	UPROPERTY()
	FString LastName;
	UPROPERTY()
	FString NickName;
	UPROPERTY()
	FString Bio;
	UPROPERTY()
	EGun EquippedGun;
	UPROPERTY()
	int Health;
	UPROPERTY()
	int MaxHealth;
	UPROPERTY()
	FGuid Id;
	UPROPERTY()
	FCrewMemberStats Stats;
	UPROPERTY()
	TArray<TSubclassOf<UBaseDD>> DirectivesAndDeviations;
};

USTRUCT()
struct SV_API FCrewPrimaries
{
	GENERATED_BODY()
public:
	FCrewPrimaries()
	{
		GunType = EGun::INVALID;
		CrewMember = FGuid::FGuid();
		Id = FGuid::FGuid();
	}

	FCrewPrimaries(EGun gun, FGuid guid = FGuid::FGuid())
	{
		GunType = gun;
		CrewMember = guid;
		Id = FGuid::NewGuid();
	}

	bool TryAssignMemberToId(FGuid memberId)
	{
		if (CrewMember == FGuid::FGuid())
		{
			CrewMember = memberId;
			return true;
		}
		return false;
	}

	void UnnasignMember() { CrewMember = FGuid::FGuid(); }

	FGuid GetPrimaryId() const { return Id; }
	FGuid GetCrewMemberId() const { return CrewMember; }
	EGun GetPrimaryGunType() const { return GunType; }

protected:
	UPROPERTY()
	EGun GunType;
	UPROPERTY()
	FGuid CrewMember;
	UPROPERTY()
	FGuid Id;
};

USTRUCT()
struct SV_API FCrewSecondaries
{
	GENERATED_BODY()
public:
	FCrewSecondaries()
	{
		GunType = EGun::INVALID;
		CrewMember = FGuid::FGuid();
		Id = FGuid::FGuid();
	}

	FCrewSecondaries(EGun gun, FGuid guid = FGuid::FGuid())
	{
		GunType = gun;
		CrewMember = guid;
		Id = FGuid::NewGuid();
	}

	bool TryAssignMemberToId(FGuid memberId)
	{
		if (CrewMember == FGuid::FGuid())
		{
			CrewMember = memberId;
			return true;
		}
		return false;
	}

	void UnnasignMember() { CrewMember = FGuid::FGuid(); }

	FGuid GetSecondaryId() const { return Id; }
	FGuid GetCrewMemberId() const { return CrewMember; }
	EGun GetSecondaryGunType() const { return GunType; }

protected:
	UPROPERTY()
	EGun GunType;
	UPROPERTY()
	FGuid CrewMember;
	UPROPERTY()
	FGuid Id;
};

USTRUCT()
struct SV_API FToolItem
{
	GENERATED_BODY()
public:
	FToolItem()
	{
	}

	FToolItem(uint8 tool, EToolType toolType, FGuid id = FGuid::NewGuid())
	{
		Tool = tool;
		ToolType = toolType;
		Id = id;
	}

	uint8 GetTool() const { return Tool; }
	EToolType GetToolType() const { return ToolType; }

protected:
	UPROPERTY()
	FGuid Id;
	UPROPERTY()
	uint8 Tool;
	UPROPERTY()
	EToolType ToolType;
};

USTRUCT()
struct SV_API FCrewTools
{
	GENERATED_BODY()
public:
	FCrewTools()
	{
	}

	FCrewTools(uint8 tool, EToolType toolType, FGuid id = FGuid::NewGuid())
	{
		ToolItem = FToolItem(tool, toolType);
		Id = id;
	}

	bool TryAssignMemberToId(FGuid memberId)
	{
		if (CrewMember == FGuid::FGuid())
		{
			CrewMember = memberId;
			return true;
		}
		return false;
	}

	void UnnasignMember() { CrewMember = FGuid::FGuid(); }

	uint8 GetTool() const { return ToolItem.GetTool(); }
	EToolType GetToolType() const { return ToolItem.GetToolType(); }

	FGuid GetToolId() const { return Id; }
	FGuid GetCrewMemberId() const { return CrewMember; }

protected:
	UPROPERTY()
	FToolItem ToolItem;
	UPROPERTY()
	FGuid CrewMember;
	UPROPERTY()
	FGuid Id;
};

USTRUCT()
struct SV_API FCurrentMission
{
	GENERATED_BODY()
public:
	FCurrentMission()
	{
		Name = "INVALID";
		Type = EMissionType::INVALID;
		Id = FGuid::FGuid();
		CurrentTurn = 1;
	}

	FCurrentMission(EMissionType missionType, FString missionName, FMissionDetails *missionDetails)
	{
		Name = missionName;
		Type = missionType;
		Id = FGuid::NewGuid();
		CurrentTurn = 1;
		MissionDetails = FMissionDetails(*missionDetails);
	}

	EMissionType GetType() const { return Type; }
	FString GetName() { return Name; }
	FGuid GetId() const { return Id; }
	int GetCurrentTurn() const { return CurrentTurn; }

	FMissionDetails *GetMissionDetails() { return &MissionDetails; }

protected:
	UPROPERTY()
	EMissionType Type;
	UPROPERTY()
	FString Name;
	UPROPERTY()
	FGuid Id;
	UPROPERTY()
	FMissionDetails MissionDetails = FMissionDetails();
	UPROPERTY()
	int CurrentTurn;
};

/**
 *
 */
USTRUCT()
struct SV_API FCurrentGameData
{
	GENERATED_BODY()
public:
	FCurrentGameData()
	{
	}

#pragma region Crew members

	FGuid AddCrewMember(FString fName, FString lName, FString bio, int health, int maxHealth)
	{
		auto crew = FCrew(fName, lName, bio, health, maxHealth);
		Crew.Emplace(crew);
		return crew.GetId();
	}
	TArray<FCrew> GetCrew()
	{
		return Crew;
	}
	FCrew *GetCrewMember(FGuid crewMemberId)
	{
		for (int i = 0; i < Crew.Num(); i++)
			if (Crew[i].GetId() == crewMemberId)
				return &Crew[i];

		return nullptr;
	}
	void SetCrewAsDead(FGuid crewMemberId)
	{
		for (int i = 0; i < Crew.Num(); i++)
			if (Crew[i].GetId() == crewMemberId)
			{
				// auto deadCrew = FCrew(Crew[i]);
				Crew.RemoveAt(i);
				return;
			}
	}

	void AddCrewDD(FGuid crewMemberId, TSubclassOf<UBaseDD> dD)
	{
		for (int i = 0; i < Crew.Num(); i++)
			if (Crew[i].GetId() == crewMemberId)
				Crew[i].AddDirectiveAndDeviation(dD);
	}

	TArray<TSubclassOf<UBaseDD>> GetCrewDD(FGuid crewMemberId)
	{
		TArray<TSubclassOf<UBaseDD>> result;
		for (int i = 0; i < Crew.Num(); i++)
			if (Crew[i].GetId() == crewMemberId)
			{
				result = Crew[i].GetDirectivesAndDeviations();
				break;
			}

		return result;
	}
#pragma endregion

#pragma region Primaries

	bool AssignPrimaryToCrew(FGuid primary, FGuid member)
	{
		for (int i = 0; i < CrewPrimaries.Num(); i++)
			if (CrewPrimaries[i].GetPrimaryId() == primary)
				return CrewPrimaries[i].TryAssignMemberToId(member);

		return false;
	}

	void UnnassignPrimaryFromCrew(FGuid primary)
	{
		for (int i = 0; i < CrewPrimaries.Num(); i++)
			if (CrewPrimaries[i].GetPrimaryId() == primary)
				CrewPrimaries[i].UnnasignMember();
	}

	FGuid AddPrimaryToCrew(EGun gunType)
	{
		auto primary = FCrewPrimaries(gunType);
		CrewPrimaries.Emplace(primary);
		return primary.GetPrimaryId();
	}

	TArray<FCrewPrimaries> GetCrewPrimaries() { return CrewPrimaries; }
	FCrewPrimaries *GetCrewPrimary(FGuid crewId)
	{
		for (int i = 0; i < CrewPrimaries.Num(); i++)
			if (CrewPrimaries[i].GetCrewMemberId() == crewId)
				return &CrewPrimaries[i];

		return nullptr;
	}
#pragma endregion

#pragma region Secondaries

	bool AssignSecondaryToCrew(FGuid primary, FGuid member)
	{
		for (int i = 0; i < CrewSecondaries.Num(); i++)
			if (CrewSecondaries[i].GetPrimaryId() == primary)
				return CrewSecondaries[i].TryAssignMemberToId(member);

		return false;
	}

	void UnnassignSecondaryFromCrew(FGuid primary)
	{
		for (int i = 0; i < CrewSecondaries.Num(); i++)
			if (CrewSecondaries[i].GetPrimaryId() == primary)
				CrewSecondaries[i].UnnasignMember();
	}

	FGuid AddSecondaryToCrew(EGun gunType)
	{
		auto primary = FCrewPrimaries(gunType);
		CrewSecondaries.Emplace(primary);
		return primary.GetPrimaryId();
	}

	TArray<FCrewPrimaries> GetCrewSecondaries() { return CrewSecondaries; }
	FCrewPrimaries *GetCrewSecondary(FGuid crewId)
	{
		for (int i = 0; i < CrewSecondaries.Num(); i++)
			if (CrewSecondaries[i].GetCrewMemberId() == crewId)
				return &CrewSecondaries[i];

		return nullptr;
	}
#pragma endregion

#pragma region Tools

	FGuid AddToolToCrew(EToolType toolType, uint8 tool)
	{
		auto newTool = FCrewTools(tool, toolType);
		CrewTools.Emplace(newTool);
		return newTool.GetToolId();
	}

	bool AssignToolToCrew(FGuid toolId, FGuid member)
	{
		for (int i = 0; i < CrewTools.Num(); i++)
			if (CrewTools[i].GetToolId() == toolId)
				return CrewTools[i].TryAssignMemberToId(member);

		return false;
	}

	void UnnassignToolFromCrew(FGuid tool)
	{
		for (int i = 0; i < CrewTools.Num(); i++)
			if (CrewTools[i].GetToolId() == tool)
				CrewTools[i].UnnasignMember();
	}

	TArray<FCrewTools *> GetToolsFromMember(FGuid memberId)
	{
		TArray<FCrewTools *> result;
		for (int i = 0; i < CrewTools.Num(); i++)
			if (CrewTools[i].GetCrewMemberId() == memberId)
				result.Emplace(&CrewTools[i]);

		return result;
	}

	TArray<FCrewTools> GetAllTools() { return CrewTools; }
	TArray<FCrewTools *> GetCrewMemberTools(FGuid crewId)
	{
		TArray<FCrewTools *> result;
		for (int i = 0; i < CrewTools.Num(); i++)
			if (CrewTools[i].GetCrewMemberId() == crewId)
				result.Emplace(&CrewTools[i]);

		return result;
	}

	FCrewTools *GetFirstUnequippedTool(EToolType toolType, uint8 tool)
	{
		for (int i = 0; i < CrewTools.Num(); i++)
			if (CrewTools[i].GetCrewMemberId() == FGuid::FGuid() &&
				CrewTools[i].GetToolType() == toolType &&
				CrewTools[i].GetTool() == tool)
				return &CrewTools[i];

		return nullptr;
	}

	void RemoveTool(FGuid toolId)
	{
		for (int i = 0; i < CrewTools.Num(); i++)
			if (CrewTools[i].GetToolId() == toolId)
				return CrewTools.RemoveAt(i);
	}

#pragma endregion

#pragma region CurrentMission

	FCurrentMission *StartNewMission(FString name, EMissionType missionType, FMissionDetails *missionDetails)
	{
		CurrentMission = FCurrentMission(missionType, name, missionDetails);
		return &CurrentMission;
	}

	FCurrentMission *GetCurrentMission() { return &CurrentMission; }

	void SetIsHistoryMission()
	{
		HistoricMissions.Emplace(FCurrentMission(CurrentMission));
	}

#pragma endregion

#pragma region WorldData

	FCurrentWorldData *GetWorldData() { return &WorldData; }

#pragma endregion

#pragma region ResourceData

	FResourceData *GetResourceData() { return &ResourceData; }

#pragma endregion

#pragma region TutorialData

	FTutorialData *GetTutorialData() { return &TutorialData; }

#pragma endregion

protected:
	UPROPERTY()
	TArray<FCrew> Crew;
	UPROPERTY()
	TArray<FCrew> DeadCrew;
	UPROPERTY()
	TArray<FCrewPrimaries> CrewPrimaries;
	UPROPERTY()
	TArray<FCrewPrimaries> CrewSecondaries;
	UPROPERTY()
	TArray<FCrewTools> CrewTools;

	UPROPERTY()
	FCurrentMission CurrentMission;
	UPROPERTY()
	TArray<FCurrentMission> HistoricMissions;
	UPROPERTY()
	FCurrentWorldData WorldData;
	UPROPERTY()
	FResourceData ResourceData;
	UPROPERTY()
	FTutorialData TutorialData;
};
