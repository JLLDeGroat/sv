// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../Enums/EEquipmentEnums.h"
#include "FResourceData.generated.h"

USTRUCT()
struct SV_API FResourceItemData
{
	GENERATED_BODY()
public:
	FResourceItemData() {
		Amount = 0;
		ResourceType = EResourceType::INVALID;
	}

	FResourceItemData(EResourceType rType) {
		Amount = 0;
		ResourceType = rType;
	}

	int GetAmount() { return Amount; }
	EResourceType GetResourceType() { return ResourceType; }

	int AddToAmount(int amount) {
		Amount += amount;
		return Amount;
	}
	int RemoveFromAmount(int amount) {
		Amount -= amount;
		return Amount;
	}
	bool HasAmount(int amount) {
		return Amount >= amount;
	}

protected:
	UPROPERTY() int Amount;
	UPROPERTY() EResourceType ResourceType;
};


USTRUCT()
struct SV_API FResourceData
{
	GENERATED_BODY()
public:
	FResourceData();
	FResourceItemData* GetResource(EResourceType resourceType);
protected:
	UPROPERTY() TArray<FResourceItemData> ResourceItems;
};
