// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Enums/EEquipmentEnums.h"
#include "FThrowableData.generated.h"

USTRUCT()
struct FThrowableDataItem
{
	GENERATED_BODY()
public:
	FThrowableDataItem() {
		ThrowableType = EThrowable::INVALID;
		Name = "INVALID";
		DamageMin = 1;
		DamageMax = 1;
		ApCost = 0;
		MaxCapacity = 4;
		Range = 600;
		Radius = 6;
		Description = "Undefined";
	}

	EThrowable GetThrowable() const { return ThrowableType; }
	FString GetName() { return Name; }

	int GetDamageMin() const { return DamageMin; }
	int GetDamageMax() const { return DamageMax; }
	int GetApCost() const { return ApCost; }

	int GetMaxCapacity() const { return MaxCapacity; }

	int GetRange() const { return Range; }
	int GetRadius() const { return Radius; }

	FString GetDescription() { return Description; }

protected:

	UPROPERTY() EThrowable ThrowableType;
	UPROPERTY() FString Name;
	UPROPERTY() int DamageMin;
	UPROPERTY() int DamageMax;
	UPROPERTY() int ApCost;
	UPROPERTY() int MaxCapacity;
	UPROPERTY() int Range;
	UPROPERTY() int Radius;
	UPROPERTY() FString Description;
};


USTRUCT()
struct FThrowableData
{
	GENERATED_BODY()
public:
	FThrowableData() {

	}

	FThrowableDataItem* GetThrowableByType(EThrowable throwable) {
		for (int i = 0; i < ThrowableDataItems.Num(); i++)
			if (ThrowableDataItems[i].GetThrowable() == throwable)
				return &ThrowableDataItems[i];

		return nullptr;
	}

protected:

	UPROPERTY() TArray<FThrowableDataItem> ThrowableDataItems;
};
