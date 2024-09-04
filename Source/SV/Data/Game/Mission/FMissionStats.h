// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMissionStats.generated.h"


USTRUCT()
struct SV_API FMissionStats
{
	GENERATED_BODY()
public:
	FMissionStats();

	void AddToKilledAmount();

	int GetKilledAmount();
protected:

	UPROPERTY() int KilledAmount;
};
