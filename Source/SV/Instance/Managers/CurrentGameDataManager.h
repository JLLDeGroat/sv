// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../Data/Game/FCurrentGameData.h"
#include "CurrentGameDataManager.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UCurrentGameDataManager : public UObject
{
	GENERATED_BODY()
	

public:

	FGuid AddCrewMember(FString Name, FString LastName, FString Bio, int Health, int MaxHealth);

	TArray<FCrew> GetEntireCrew();
	FCurrentGameData* GetCurrentGameData();

private:
	FCurrentGameData CurrentGameData;
};
