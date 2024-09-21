// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseInstanceManager.h"
#include "../../Data/FThrowableData.h"
#include "ThrowableDataManager.generated.h"

/**
 *
 */
UCLASS()
class SV_API UThrowableDataManager : public UBaseInstanceManager
{
	GENERATED_BODY()

public:

	UThrowableDataManager();
	FThrowableData* GetThrowableData();

protected:

	UPROPERTY() FThrowableData ThrowableData;

};
