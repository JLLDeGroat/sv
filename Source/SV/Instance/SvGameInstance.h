// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../Data/SkillData.h"
#include "../Data/FThrowableData.h"
#include "../Data/FGameTypeDescriptions.h"
#include "../Data/FCrewMemberData.h"
#include "../Data/FMissionNameData.h"
#include "SvGameInstance.generated.h"

class URouteDataManager;
class ULevelGenerationManager;
class UMissionDetailsManager;
class UCurrentGameDataManager;
class USupplyDataManager;
class UWeaponDataManager;
class UThrowableDataManager;
class UHealthKitDataManager;

UCLASS()
class SV_API USvGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	USvGameInstance();

	void GetSkillDataItem(FString name, FSkillDataItem& dataItem);
	void GetGameTypeDescription(EGameModeType gameMode, FGameTypeDescriptionItem& item);

	URouteDataManager* GetRouteDataManager();

	//gets data loaded from file, all names/last names and bios
	FCrewMemberData* GetPossibleCrewData();
	UMissionDetailsManager* GetMissionDetailsManager();
	UCurrentGameDataManager* GetCurrentGameDataManager();
	USupplyDataManager* GetSupplyDataManager();
	UWeaponDataManager* GetWeaponDataManager();
	UThrowableDataManager* GetThrowableDataManager();
	UHealthKitDataManager* GetHealthKitDataManager();
protected:

	UPROPERTY() FSkillData SkillData;
	UPROPERTY() FGameTypeDescriptions GameTypeDescriptions;
	UPROPERTY() FCrewMemberData CrewMemberData;

	template<typename OutStructType>
	bool ReadFile(FString file, OutStructType* OutStruct);

	virtual void Shutdown() override;

private:

	void ReadModData(FString modName, FString& responseString);
	UPROPERTY() URouteDataManager* RouteManager;
	UPROPERTY() ULevelGenerationManager* LevelGenManager;
	UPROPERTY() UMissionDetailsManager* MissionManager;
	UPROPERTY() UCurrentGameDataManager* CurrentGameDataManager;
	UPROPERTY() USupplyDataManager* SupplyDataManager;
	UPROPERTY() UWeaponDataManager* WeaponDataManager;
	UPROPERTY() UThrowableDataManager* ThrowableDataManager;
	UPROPERTY() UHealthKitDataManager* HealthKitDataManager;
};
