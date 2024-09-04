// Fill out your copyright notice in the Description page of Project Settings.


#include "FMissionDetails.h"

FMissionDetails::FMissionDetails() {
	bValidMission = false;
	bIsCompleted = false;
	Turn = 0;
	MissionStats = FMissionStats();
}

FMissionDetails::FMissionDetails(FString name, FString desc, EMissionType type, FString missionTypeDescription) {
	Name = name;
	Description = desc;
	MissionType = type;
	bValidMission = true;
	MissionTypeDescription = missionTypeDescription;
	bIsCompleted = false;
	Turn = 0;
	MissionStats = FMissionStats();
}

FString FMissionDetails::GetName() {
	return Name;
}
FString FMissionDetails::GetDescription() {
	return Description;
}
FString FMissionDetails::GetMissionTypeDescription() {
	return MissionTypeDescription;
}
EMissionType FMissionDetails::GetMissionType() {
	return MissionType;
}
bool FMissionDetails::GetIsValidMission() {
	return bValidMission;
}
bool FMissionDetails::GetIsCompleted() {
	return bIsCompleted;
}

void FMissionDetails::SetName(FString name) {
	Name = name;
}
void FMissionDetails::SetDescription(FString desc) {
	Description = desc;
}
void FMissionDetails::SetMissionType(EMissionType missionType) {
	MissionType = missionType;
}
void FMissionDetails::SetMissionTypeDescription(FString missionTypeDesc) {
	MissionTypeDescription = missionTypeDesc;
}
void FMissionDetails::SetIsValidMission(bool val) {
	bValidMission = val;
}
void FMissionDetails::SetIsCompleted(bool val) {
	bIsCompleted = val;
}
FString FMissionDetails::GetFluffText() {
	return FluffText;
}
void FMissionDetails::SetFluffText(FString txt) {
	FluffText = txt;
}
FString FMissionDetails::GetMainObjective() {
	if (MissionType == EMissionType::MT_Survive) {
		return MainObjective + " (Turns: " + FString::SanitizeFloat(TurnLimit - Turn, 0) + ")";
	}
	return MainObjective;
}
void FMissionDetails::SetMainObjective(FString txt) {
	MainObjective = txt;
}

int FMissionDetails::GetTurn() {
	return Turn;
}
void FMissionDetails::AddToTurnCounter() {
	Turn += 1;
}

void FMissionDetails::SetTurnLimit(int turnLimit) {
	TurnLimit = turnLimit;
}
int FMissionDetails::GetTurnLimit() {
	return TurnLimit;
}

TMap<EResourceType, int> FMissionDetails::GetResourceExtracted() {
	return ResourceExtracted;
}
bool FMissionDetails::GetExtractedIntel() {
	return bExtractedIntel;
}

void FMissionDetails::AddToResourceExtracted(EResourceType rType, int amount) {
	if (ResourceExtracted.Find(rType)) {
		ResourceExtracted[rType] += amount;
	}
	else {
		ResourceExtracted.Add(rType);
		ResourceExtracted[rType] += amount;
	}
}
void FMissionDetails::SetHasExtractedIntel() {
	bExtractedIntel = true;
}

FMissionStats* FMissionDetails::GetMissionStats() {
	return &MissionStats;
}