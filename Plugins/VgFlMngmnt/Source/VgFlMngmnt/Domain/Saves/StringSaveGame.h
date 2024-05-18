// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../../Data/FStringSaveData.h"
#include "StringSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class VGFLMNGMNT_API UStringSaveGame : public USaveGame
{
	GENERATED_BODY()

public:


	void SetSaveData(FString str) {
		StringSaveData = FStringSaveData();
		StringSaveData.SetData(str);
	}

	FString GetSaveData() {
		return StringSaveData.GetData();
	}

protected:

	UPROPERTY() FStringSaveData StringSaveData;
	
};
