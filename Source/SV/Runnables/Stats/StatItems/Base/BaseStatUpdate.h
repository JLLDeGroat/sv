// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../../../Enums/EWorldEnums.h"
#include "BaseStatUpdate.generated.h"

struct FCrewMemberStats;
struct FMissionStats;

/**
 *
 */
UCLASS()
class SV_API UBaseStatUpdate : public UObject
{
	GENERATED_BODY()

public:

	UBaseStatUpdate* Setup(AActor* statActor, EStatisticType sType, float value = 0.0f) {
		StatActor = statActor;
		StatType = sType;
		StatValue = value;
		return this;
	}

	virtual void ActivateStatUpdate();

protected:

	UPROPERTY() AActor* StatActor;
	UPROPERTY() EStatisticType StatType;
	UPROPERTY() float StatValue;

	FCrewMemberStats* GetCrewMemberStats();
	FMissionStats* GetCurrentMissionStats();

};
