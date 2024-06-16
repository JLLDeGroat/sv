// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../Data/SkillData.h"
#include "../Data/FThrowableData.h"
#include "../Data/FGameTypeDescriptions.h"
#include "../Data/FGameData.h"
#include "../Data/FCrewMemberData.h"
#include "SvGameInstance.generated.h"

class URouteDataManager;
class UCrewDataManager;
class ULevelGenerationManager;
/**
 * 
 */
UCLASS()
class SV_API USvGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	USvGameInstance();

	void GetSkillDataItem(FString name, FSkillDataItem& dataItem);
	void GetThrowableDataItem(EThrowable throwable, FThrowableDataItem& item);
	void GetGameTypeDescription(EGameModeType gameMode, FGameTypeDescriptionItem& item);

	URouteDataManager* GetRouteDataManager();
	//gets data object that handles to current crew
	UCrewDataManager* GetCrewManager();

	//gets data loaded from file, all names/last names and bios
	FCrewMemberData* GetPossibleCrewData();
protected:

	UPROPERTY() FSkillData SkillData;
	UPROPERTY() FThrowableData ThrowableData;
	UPROPERTY() FGameTypeDescriptions GameTypeDescriptions;
	UPROPERTY() FCrewMemberData CrewMemberData;

	UPROPERTY() FGameData GameData;

	template<typename OutStructType>
	bool ReadFile(FString file, OutStructType* OutStruct);

private:

	void ReadModData(FString modName, FString& responseString);
	UPROPERTY() URouteDataManager* RouteManager;
	UPROPERTY() UCrewDataManager* CrewDataManager;
	UPROPERTY() ULevelGenerationManager* LevelGenManager;

};
