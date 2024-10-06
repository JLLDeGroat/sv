// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Enums/EEquipmentEnums.h"
#include "FHealthKitData.generated.h"

USTRUCT()
struct FHealthKitDataItem
{
	GENERATED_BODY()
public:
	FHealthKitDataItem() {
		Name = "Untitled";
		HealAmount = 0;
	}

	FString GetName() { return Name; }
	FString GetDescription() { return Description; }
	EHealthKits GetHealthKitType() { return HealthKitType; }

	float GetHealAmount() { return HealAmount; }
protected:
	UPROPERTY() FString Name;
	UPROPERTY() FString Description;
	UPROPERTY() EHealthKits HealthKitType;
	UPROPERTY() float HealAmount;
};


USTRUCT()
struct FHealthKitData
{
	GENERATED_BODY()
public:
	FHealthKitData() {

	}

	FHealthKitDataItem* GetHealthKit(EHealthKits healthKitType) {
		if (healthKitType == EHealthKits::INVALID) return nullptr;

		for (int i = 0; i < HealthKitItems.Num(); i++)
			if (HealthKitItems[i].GetHealthKitType() == healthKitType)
				return &HealthKitItems[i];

		return nullptr;
	}

protected:

	UPROPERTY() TArray<FHealthKitDataItem> HealthKitItems;
};
