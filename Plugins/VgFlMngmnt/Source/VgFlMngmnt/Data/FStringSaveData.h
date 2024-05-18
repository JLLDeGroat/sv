// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FStringSaveData.generated.h"
/**
 * 
 */
USTRUCT()
struct VGFLMNGMNT_API FStringSaveData
{
	GENERATED_BODY()
public:
	FStringSaveData() {
		Data = "";
	}
	
	void SetData(FString data) {
		Data = data;
	}

	FString GetData() {
		return Data;
	}

protected:

	UPROPERTY() FString Data;
};
