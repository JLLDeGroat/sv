// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../Enums/EEquipmentEnums.h"

#include "FCurrentGameData.generated.h"


USTRUCT()
struct SV_API FCrew
{
	GENERATED_BODY()
public:
	FCrew() {
		LastName = "";
		FirstName = "";
		NickName = "";
		EquippedGun = EGun::INVALID;
		Health = 0;
		MaxHealth = 0;
		Bio = "";
	}

	FCrew(FString fName, FString lName, FString bio, int health, int maxHealth, FGuid guid = FGuid::NewGuid()) {
		LastName = fName;
		FirstName = lName;
		NickName = "";
		EquippedGun = EGun::INVALID;
		Health = health;
		MaxHealth = maxHealth;
		Bio = bio;
		Id = guid;
	}

	FString GetName() const { return FirstName + " " + LastName; }
	FGuid GetId() const { return Id; }

protected:
	UPROPERTY() FString FirstName;
	UPROPERTY() FString LastName;
	UPROPERTY() FString NickName;
	UPROPERTY() FString Bio;

	UPROPERTY() EGun EquippedGun;

	UPROPERTY() int Health;
	UPROPERTY() int MaxHealth;

	UPROPERTY() FGuid Id;
};


USTRUCT()
struct SV_API FCrewPrimaries
{
	GENERATED_BODY()
public:
	FCrewPrimaries() {

	}

	FCrewPrimaries(EGun gun, FGuid guid = FGuid::FGuid()) {
		GunType = gun;
		CrewMember = guid;
		Id = FGuid::NewGuid();
	}

	bool TryAssignMemberToId(FGuid memberId) {
		if (CrewMember == FGuid::FGuid()) {
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
	UPROPERTY() EGun GunType;
	UPROPERTY() FGuid CrewMember;
	UPROPERTY() FGuid Id;
};


USTRUCT()
struct SV_API FToolItem
{
	GENERATED_BODY()
public:
	FToolItem() {

	}

	FToolItem(uint8 tool, EToolType toolType, FGuid id = FGuid::NewGuid()) {
		Tool = tool;
		ToolType = toolType;
		Id = id;
	}

	uint8 GetTool() const { return Tool; }
	EToolType GetToolType() const { return ToolType; }
protected:
	UPROPERTY() FGuid Id;
	UPROPERTY() uint8 Tool;
	UPROPERTY() EToolType ToolType;
};

USTRUCT()
struct SV_API FCrewTools
{
	GENERATED_BODY()
public:
	FCrewTools() {

	}

	FCrewTools(uint8 tool, EToolType toolType, FGuid id = FGuid::NewGuid()) {
		ToolItem = FToolItem(tool, toolType);
		Id = id;
	}

	bool TryAssignMemberToId(FGuid memberId) {
		if (CrewMember == FGuid::FGuid()) {
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

	UPROPERTY() FToolItem ToolItem;
	UPROPERTY() FGuid CrewMember;
	UPROPERTY() FGuid Id;
};


/**
 *
 */
USTRUCT()
struct SV_API FCurrentGameData
{
	GENERATED_BODY()
public:
	FCurrentGameData() {

	}

#pragma region Crew members

	FGuid AddCrewMember(FString fName, FString lName, FString bio, int health, int maxHealth) {
		auto crew = FCrew(fName, lName, bio, health, maxHealth);
		Crew.Emplace(crew);
		return crew.GetId();
	}
	TArray<FCrew> GetCrew() {
		return Crew;
	}
	FCrew GetCrewMember(FGuid crewMemberId) {
		for (int i = 0; i < Crew.Num(); i++)
			if (Crew[i].GetId() == crewMemberId)
				return Crew[i];

		return FCrew();
	}
#pragma endregion

#pragma region Primaries

	bool AssignPrimaryToCrew(FGuid primary, FGuid member) {
		for (int i = 0; i < CrewPrimaries.Num(); i++)
			if (CrewPrimaries[i].GetPrimaryId() == primary)
				return CrewPrimaries[i].TryAssignMemberToId(member);

		return false;
	}

	void UnnassignPrimaryFromCrew(FGuid primary) {
		for (int i = 0; i < CrewPrimaries.Num(); i++)
			if (CrewPrimaries[i].GetPrimaryId() == primary)
				CrewPrimaries[i].UnnasignMember();
	}

	FGuid AddPrimaryToCrew(EGun gunType) {
		auto primary = FCrewPrimaries(gunType);
		CrewPrimaries.Emplace(primary);
		return primary.GetPrimaryId();
	}

	TArray<FCrewPrimaries> GetCrewPrimaries() { return CrewPrimaries; }
	FCrewPrimaries GetCrewPrimary(FGuid crewId) {
		for (int i = 0; i < CrewPrimaries.Num(); i++)
			if (CrewPrimaries[i].GetCrewMemberId() == crewId)
				return CrewPrimaries[i];

		return FCrewPrimaries();
	}
#pragma endregion

#pragma region Tools

	FGuid AddToolToCrew(EToolType toolType, uint8 tool) {
		auto newTool = FCrewTools(tool, toolType);
		CrewTools.Emplace(newTool);
		return newTool.GetToolId();
	}

	bool AssignToolToCrew(FGuid toolId, FGuid member) {
		for (int i = 0; i < CrewTools.Num(); i++)
			if (CrewTools[i].GetToolId() == toolId)
				return CrewTools[i].TryAssignMemberToId(member);

		return false;
	}

	void UnnassignToolFromCrew(FGuid tool) {
		for (int i = 0; i < CrewTools.Num(); i++)
			if (CrewTools[i].GetToolId() == tool)
				CrewTools[i].UnnasignMember();
	}

	TArray<FCrewTools*> GetToolsFromMember(FGuid memberId) {
		TArray<FCrewTools*> result;
		for (int i = 0; i < CrewTools.Num(); i++)
			if (CrewTools[i].GetCrewMemberId() == memberId)
				result.Emplace(&CrewTools[i]);

		return result;
	}

	TArray<FCrewTools> GetAllTools() { return CrewTools; }
	TArray<FCrewTools*> GetCrewMemberTools(FGuid crewId) {
		TArray<FCrewTools*> result;
		for (int i = 0; i < CrewTools.Num(); i++)
			if (CrewTools[i].GetCrewMemberId() == crewId)
				result.Emplace(&CrewTools[i]);

		return result;
	}

	FCrewTools* GetFirstUnequippedTool(EToolType toolType, uint8 tool) {
		for (int i = 0; i < CrewTools.Num(); i++)
			if (CrewTools[i].GetCrewMemberId() == FGuid::FGuid() &&
				CrewTools[i].GetToolType() == toolType &&
				CrewTools[i].GetTool() == tool)
				return &CrewTools[i];

		return nullptr;
	}

#pragma endregion

protected:
	UPROPERTY() TArray<FCrew> Crew;
	UPROPERTY() TArray<FCrewPrimaries> CrewPrimaries;
	UPROPERTY() TArray<FCrewTools> CrewTools;
};