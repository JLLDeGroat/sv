// Fill out your copyright notice in the Description page of Project Settings.


#include "SvGameInstance.h"
#include "../Data/SkillData.h"
#include "Misc/Paths.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "VgFlMngmnt/Domain/FileManagementUtilities.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"

USvGameInstance::USvGameInstance() {

	FString skillStringData;
	FSkillData skillData;
	ReadModData("Base/sv_sd", skillStringData);
	if (!UFileManagementUtilities::JsonToStruct(skillStringData, &SkillData)) {
		UDebugMessages::LogError(this, "failed to load sv_sd");
		return;
	} 
}

void USvGameInstance::ReadModData(FString modName, FString& fileText) {
	FString file = FPaths::ProjectModsDir() + modName + ".json";
	UDebugMessages::LogDisplay(this, file);
	FString returnString;
	if (!FFileHelper::LoadFileToString(returnString, *file)) {
		UDebugMessages::LogError(this, "failed to load mod data " + modName);
		return;
	}

	fileText = returnString;
}

void USvGameInstance::GetSkillDataItem(FString name, FSkillDataItem& dataItem) {
	auto foundDataItem = SkillData.GetItemByName(name);
	dataItem = *foundDataItem;
}