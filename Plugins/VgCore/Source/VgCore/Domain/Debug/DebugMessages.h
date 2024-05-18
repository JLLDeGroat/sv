// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DebugMessages.generated.h"

/**
 * 
 */
UCLASS()
class VGCORE_API UDebugMessages : public UObject
{
	GENERATED_BODY()
	
public:

	static void DisplayMessage(FString msg);

	static void LogDisplay(UObject* contextObj, FString msg);
	static void LogWarning(UObject* contextObj, FString msg);
	static void LogError(UObject* contextObj, FString msg);
};
