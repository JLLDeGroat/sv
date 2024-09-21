// Fill out your copyright notice in the Description page of Project Settings.


#include "SvGameInstance.h"
#include "../Data/SkillData.h"
#include "Misc/Paths.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "VgFlMngmnt/Domain/FileManagementUtilities.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"
#include "Managers/RouteDataManager.h"
#include "Managers/LevelGenerationManager.h"
#include "Managers/MissionDetailsManager.h"
#include "Managers/CurrentGameDataManager.h"
#include "Managers/SupplyDataManager.h"
#include "Managers/WeaponDataManager.h"
#include "Managers/ThrowableDataManager.h"

USvGameInstance::USvGameInstance() {

	FString skillStringData;

	ReadFile("Base/sv_sd", &SkillData);
	ReadFile("Base/sv_gtd", &GameTypeDescriptions);
	ReadFile("Base/sv_cmd", &CrewMemberData);

	RouteManager = NewObject<URouteDataManager>();
	LevelGenManager = NewObject<ULevelGenerationManager>();
	MissionManager = NewObject<UMissionDetailsManager>();
	CurrentGameDataManager = NewObject<UCurrentGameDataManager>();
	SupplyDataManager = NewObject<USupplyDataManager>();
	WeaponDataManager = NewObject<UWeaponDataManager>();
	ThrowableDataManager = NewObject<UThrowableDataManager>();
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

void USvGameInstance::GetGameTypeDescription(EGameModeType gameMode, FGameTypeDescriptionItem& item) {
	auto foundDescriptionItem = GameTypeDescriptions.GetDescriptionItem(gameMode);
	item = *foundDescriptionItem;
}

template<typename OutStructType>
bool USvGameInstance::ReadFile(FString file, OutStructType* OutStruct)
{
	FString fileText;
	ReadModData(file, fileText);
	return UFileManagementUtilities::JsonToStruct(fileText, OutStruct);
}

URouteDataManager* USvGameInstance::GetRouteDataManager() {
	return RouteManager;
}

FCrewMemberData* USvGameInstance::GetPossibleCrewData() {
	return &CrewMemberData;
}

UMissionDetailsManager* USvGameInstance::GetMissionDetailsManager() {
	return MissionManager;
}

UCurrentGameDataManager* USvGameInstance::GetCurrentGameDataManager() {
	return CurrentGameDataManager;
}

USupplyDataManager* USvGameInstance::GetSupplyDataManager() {
	return SupplyDataManager;
}
UWeaponDataManager* USvGameInstance::GetWeaponDataManager() {
	return WeaponDataManager;
}
UThrowableDataManager* USvGameInstance::GetThrowableDataManager() {
	return ThrowableDataManager;
}