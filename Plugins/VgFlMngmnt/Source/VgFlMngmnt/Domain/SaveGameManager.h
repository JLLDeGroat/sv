// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Interfaces/SaveManager.h"
#include "SaveGameManager.generated.h"

/**
 * 
 */
UCLASS()
class VGFLMNGMNT_API USaveGameManager : public UObject, public ISaveManager
{
	GENERATED_BODY()
	
public:

	virtual void SaveGameData(FString jsonString) override;
	virtual FString LoadGameData() override;

	virtual void PurgeGameData() override;

	virtual void SaveToFile(FString file, FString jsonString) override;
	virtual FString LoadFile(FString file) override;

	virtual void PurgeFileData(FString file) override;

private:

	UPROPERTY() FString SaveGameDataLoc = "__GameData__";

};
