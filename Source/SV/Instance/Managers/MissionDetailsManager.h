// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseInstanceManager.h"
#include "../../Data/FMissionNameData.h"
#include "../../Enums/EWorldEnums.h"
#include "MissionDetailsManager.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UMissionDetailsManager : public UBaseInstanceManager
{
	GENERATED_BODY()

public:

	UMissionDetailsManager();
	FString GenerateMissionName();

	FMissionDescriptions* GetMissionDescription(EMissionType missionType);

protected:

	UPROPERTY() FMissionNameData MissionNameData;
};
