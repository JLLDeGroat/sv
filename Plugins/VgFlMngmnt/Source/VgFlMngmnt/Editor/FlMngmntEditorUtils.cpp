// Fill out your copyright notice in the Description page of Project Settings.


#include "FlMngmntEditorUtils.h"
#include "../Domain/FileManagementUtilities.h"

void UFlMngmntEditorUtils::PurgeGameData() {
	UFileManagementUtilities::GetSaveManager()->PurgeGameData();
}

void UFlMngmntEditorUtils::PurgeFileData(const FString& fileName) {
	UFileManagementUtilities::GetSaveManager()->PurgeFileData(fileName);
}