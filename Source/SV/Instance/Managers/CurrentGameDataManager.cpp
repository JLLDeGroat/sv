// Fill out your copyright notice in the Description page of Project Settings.


#include "CurrentGameDataManager.h"

void UCurrentGameDataManager::AddCrewMember(FString Name, FString LastName, FString Bio, int Health, int MaxHealth) {
	CurrentGameData.AddCrewMember(Name, LastName, Bio, Health, MaxHealth);
}

TArray<FCrew> UCurrentGameDataManager::GetEntireCrew() {
	return CurrentGameData.GetCrew();
}
FCurrentGameData* UCurrentGameDataManager::GetCurrentGameData() {
	return &CurrentGameData;
}