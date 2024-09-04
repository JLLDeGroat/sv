// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionDetailsManager.h"

UMissionDetailsManager::UMissionDetailsManager() {
	ReadFile("Base/sv_md", &MissionNameData);
}

FString UMissionDetailsManager::GenerateMissionName() {
	auto missionTitles = MissionNameData.GetMissionTitles();
	auto adjectives = missionTitles->GetMissionAdjectives();
	auto nouns = missionTitles->GetMissionNouns();

	return adjectives[FMath::RandRange(0, adjectives.Num() - 1)] +
		" " +
		nouns[FMath::RandRange(0, nouns.Num() - 1)];
}

FMissionDescriptions* UMissionDetailsManager::GetMissionTypeDescription(EMissionType missionType) {
	return MissionNameData.GetDescription(missionType);
}
FString UMissionDetailsManager::GenerateFluffText(EMissionType missionType) {
	return MissionNameData.GetRandomFluff(missionType);
}
FString UMissionDetailsManager::GenerateMainObjective(EMissionType missionType) {
	return MissionNameData.GetRandomMainObjective(missionType);
}