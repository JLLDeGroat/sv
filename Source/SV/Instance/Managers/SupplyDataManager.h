// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseInstanceManager.h"
#include "../../Data/FSupplyStockData.h"
#include "SupplyDataManager.generated.h"



/**
 *
 */
UCLASS()
class SV_API USupplyDataManager : public UBaseInstanceManager
{
	GENERATED_BODY()

public:

	USupplyDataManager();

	FSupplyStockData* GetSupplyStockData();

protected:

	UPROPERTY() FSupplyStockData SupplyStockData;

};
