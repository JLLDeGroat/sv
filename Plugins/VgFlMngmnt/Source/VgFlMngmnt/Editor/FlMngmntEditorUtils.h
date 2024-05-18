// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FlMngmntEditorUtils.generated.h"

/**
 * 
 */
UCLASS()
class VGFLMNGMNT_API UFlMngmntEditorUtils : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable) static void PurgeGameData();


	UFUNCTION(BlueprintCallable) static void PurgeFileData(const FString& fileName);
};
