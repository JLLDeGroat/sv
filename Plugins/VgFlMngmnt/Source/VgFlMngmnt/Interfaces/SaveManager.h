// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UObject/Interface.h"
#include "CoreMinimal.h"
#include "SaveManager.generated.h"

/**
 * 
 */
UINTERFACE(BlueprintType)
class VGFLMNGMNT_API USaveManager : public UInterface
{
	GENERATED_BODY()
public:


};


class VGFLMNGMNT_API ISaveManager 
{

	GENERATED_BODY()

public:
	
	virtual void SaveGameData(FString jsonString) = 0;
	virtual FString LoadGameData() = 0;

	virtual void PurgeGameData() = 0;

	virtual void SaveToFile(FString file, FString jsonString) = 0;
	virtual FString LoadFile(FString file) = 0;

	virtual void PurgeFileData(FString file) = 0;
	//Save ProfileData

};