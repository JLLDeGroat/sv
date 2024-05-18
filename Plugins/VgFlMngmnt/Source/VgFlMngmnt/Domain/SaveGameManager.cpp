// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameManager.h"
#include "../Data/FStringSaveData.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Saves/StringSaveGame.h"

void USaveGameManager::SaveGameData(FString jsonString) {
	auto newSave = NewObject<UStringSaveGame>();
	newSave->SetSaveData(jsonString);

	UGameplayStatics::SaveGameToSlot(newSave, *SaveGameDataLoc, 0);
}

FString USaveGameManager::LoadGameData() {
	if (UGameplayStatics::DoesSaveGameExist(*SaveGameDataLoc, 0)) {
		auto loadSave = Cast<UStringSaveGame>(UGameplayStatics::LoadGameFromSlot(*SaveGameDataLoc, 0));
		return loadSave->GetSaveData();
	}

	return "";
}

void USaveGameManager::PurgeGameData() {
	UGameplayStatics::DeleteGameInSlot(*SaveGameDataLoc, 0);
}


void USaveGameManager::SaveToFile(FString file, FString jsonString) {
	auto newSave = NewObject<UStringSaveGame>();
	newSave->SetSaveData(jsonString);

	UGameplayStatics::SaveGameToSlot(newSave, *file, 0);
}

FString USaveGameManager::LoadFile(FString file) {
	if (UGameplayStatics::DoesSaveGameExist(*file, 0)) {
		auto loadSave = Cast<UStringSaveGame>(UGameplayStatics::LoadGameFromSlot(*file, 0));
		return loadSave->GetSaveData();
	}

	return "";
}

void USaveGameManager::PurgeFileData(FString file) {
	UGameplayStatics::DeleteGameInSlot(*file, 0);
}