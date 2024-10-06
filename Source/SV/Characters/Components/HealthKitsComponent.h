// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/AnimAccessComponent.h"
#include "../../Data/FHealthKitData.h"
#include "HealthKitsComponent.generated.h"

USTRUCT()
struct FHealthKit
{
	GENERATED_BODY()

public:
	FHealthKit() {
		Amount = 0;
		HealthKitData = FHealthKitDataItem();
		kitId = FGuid::FGuid();
	}

	FHealthKit(FHealthKitDataItem healthKitData, int amount, FGuid id) {
		Amount = amount;
		HealthKitData = healthKitData;
		kitId = id;
	}

	void Remove(int amount) {
		amount -= 1;
	}

	void Add(int amount) {
		Amount += amount;
	}

	FHealthKitDataItem* GetHealthKitDataItem() { return &HealthKitData; }
	int GetAmount() const { return Amount; }
	FGuid GetHealthKitId() { return kitId; }

protected:
	UPROPERTY() FHealthKitDataItem HealthKitData;
	UPROPERTY() int Amount;
	UPROPERTY() FGuid kitId;
};

UCLASS()
class SV_API UHealthKitsComponent : public UAnimAccessComponent
{
	GENERATED_BODY()

public:

	UHealthKitsComponent(const FObjectInitializer& ObjectInitializer);

	int GetHealthKitAmounts(EHealthKits healthKit);

	void AddHealthKits(EHealthKits healthKit, int amount, FGuid kitId);

	void AddKitoPreviouslyUsed(EHealthKits healthKit, int amount, FGuid kitId);
	TArray<FHealthKit> GetPreviouslyThrownThrowables();

	void SetActiveHealthKit(EHealthKits healthKit);
	EHealthKits GetActiveHealthKit();

	void BeginUseActiveHealthKitOnActor(AActor* actor);
	void UseActiveHealthKitOnActor();

	void SpawnHealthKitOnHand();
	void DespawnHealthKitOnHand();

	AActor* GetHealthKitActor();

protected:
	UPROPERTY() TArray<FHealthKit> HealthKits;
	UPROPERTY() TArray<FHealthKit> UsedHealthKits;

	UPROPERTY() AActor* ActiveHealthKitRecipient;
	UPROPERTY() EHealthKits ActiveHealthKit;

	UPROPERTY() AActor* HealthKitActor;
};
