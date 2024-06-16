// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FCrewMemberData.generated.h"

/**
 *
 */
USTRUCT()
struct FCrewMemberData
{
	GENERATED_BODY()
public:
	FCrewMemberData() {

	}

	TArray<FString> GetFirstNames() { return FirstNames; }
	TArray<FString> GetLastNames() { return LastNames; }
	TArray<FString> GetBios() { return Bios; }

protected:
	UPROPERTY() TArray<FString> FirstNames;
	UPROPERTY() TArray<FString> LastNames;
	UPROPERTY() TArray<FString> Bios;
};
