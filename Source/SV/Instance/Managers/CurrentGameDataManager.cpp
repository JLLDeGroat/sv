// Fill out your copyright notice in the Description page of Project Settings.


#include "CurrentGameDataManager.h"

FGuid UCurrentGameDataManager::AddCrewMember(FString Name, FString LastName, FString Bio, int Health, int MaxHealth) {
	auto memberId = CurrentGameData.AddCrewMember(Name, LastName, Bio, Health, MaxHealth);
	return memberId;
}

TArray<FCrew> UCurrentGameDataManager::GetEntireCrew() {
	return CurrentGameData.GetCrew();
}
FCurrentGameData* UCurrentGameDataManager::GetCurrentGameData() {
	return &CurrentGameData;
}