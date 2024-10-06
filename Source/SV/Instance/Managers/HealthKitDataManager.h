// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseInstanceManager.h"
#include "../../Data/FHealthKitData.h"
#include "HealthKitDataManager.generated.h"

/**
 *
 */
UCLASS()
class SV_API UHealthKitDataManager : public UBaseInstanceManager
{
	GENERATED_BODY()

public:

	UHealthKitDataManager();
	FHealthKitData* GetHealthKitData();

protected:

	UPROPERTY() FHealthKitData HealthKitData;
};
