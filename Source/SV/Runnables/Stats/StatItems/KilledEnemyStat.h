// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseStatUpdate.h"
#include "KilledEnemyStat.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UKilledEnemyStat : public UBaseStatUpdate
{
	GENERATED_BODY()

public:

	virtual void ActivateStatUpdate() override;
	
};
