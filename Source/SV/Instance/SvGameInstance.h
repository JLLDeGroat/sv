// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../Data/SkillData.h"
#include "SvGameInstance.generated.h"

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
	
protected:

	UPROPERTY() FSkillData SkillData;

private:

	void ReadModData(FString modName, FString& responseString);

};