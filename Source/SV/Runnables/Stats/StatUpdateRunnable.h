// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/BaseRunnable.h"
#include "../../Enums/EWorldEnums.h"
#include "StatUpdateRunnable.generated.h"

/**
 *
 */
UCLASS()
class SV_API UStatUpdateRunnable : public UBaseRunnable
{
	GENERATED_BODY()

public:

	virtual void ActivateThread() override;
	UBaseRunnable* InsertVariables(AActor* statOwner, EStatisticType statType, float value = 0.0f) {
		StatOwner = statOwner;
		StatType = statType;
		StatValue = value;
		return this;
	}

	bool GetIsCompleteStatItem() { return bCompleteStatItem; }

protected:

	UPROPERTY() AActor* StatOwner;
	UPROPERTY() EStatisticType StatType;
	UPROPERTY() float StatValue;

	UPROPERTY() bool bCompleteStatItem;

};