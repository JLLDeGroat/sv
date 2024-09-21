#pragma once
#include "CoreMinimal.h"
#include "../Enums/EConfigurationEnums.h"
#include "../Enums/EEquipmentEnums.h"
#include "FSupplyStockData.generated.h"

USTRUCT()
struct FSupplyStockItem
{
	GENERATED_BODY()
public:
	FSupplyStockItem() {

	}

	TArray<EStockCategory> GetStockCategories() { return StockCategories; }
	TMap<EResourceType, int> GetCosts() { return Costs; }

	EThrowable GetThrowableType() { return ThrowableType; }
	EGun GetGunType() { return GunType; }
	EToolType GetToolType() { return ToolType; }
protected:

	UPROPERTY() TArray<EStockCategory> StockCategories;
	UPROPERTY() TMap<EResourceType, int> Costs;

	UPROPERTY() EThrowable ThrowableType;
	UPROPERTY() EGun GunType;
	UPROPERTY() EToolType ToolType;
};


USTRUCT()
struct FSupplyStockData
{
	GENERATED_BODY()
public:
	FSupplyStockData() {

	}

	TArray<FSupplyStockItem> GetStockItems() { return StockItems; }

protected:

	UPROPERTY() TArray<FSupplyStockItem> StockItems;
};
