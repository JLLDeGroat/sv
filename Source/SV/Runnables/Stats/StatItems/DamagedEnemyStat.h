// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseStatUpdate.h"
#include "DamagedEnemyStat.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UDamagedEnemyStat : public UBaseStatUpdate
{
	GENERATED_BODY()
	
public:

	virtual void ActivateStatUpdate() override;
};
