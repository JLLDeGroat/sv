// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VgFlMngmnt/Domain/FileManagementUtilities.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"
#include "BaseInstanceManager.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UBaseInstanceManager : public UObject
{
	GENERATED_BODY()


public:

	template<typename OutStructType>
	bool ReadFile(FString file, OutStructType* OutStruct)
	{
		FString fileText;
		ReadModData(file, fileText);
		return UFileManagementUtilities::JsonToStruct(fileText, OutStruct);
	}

	void ReadModData(FString modName, FString& fileText);
};
