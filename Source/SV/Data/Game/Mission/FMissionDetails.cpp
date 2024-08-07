// Fill out your copyright notice in the Description page of Project Settings.


#include "FMissionDetails.h"

FMissionDetails::FMissionDetails()
{
	bValidMission = false;
	bIsCompleted = false;
}

FMissionDetails::FMissionDetails(FString name, FString desc, EMissionType type, FString missionTypeDescription) {
	Name = name;
	Description = desc;
	MissionType = type;
	bValidMission = true;
	MissionTypeDescription = missionTypeDescription;
	bIsCompleted = false;
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
void FMissionDetails::SetIsValidMission(bool val) {
	bValidMission = val;
}
void FMissionDetails::SetIsCompleted(bool val) {
	bIsCompleted = val;
}