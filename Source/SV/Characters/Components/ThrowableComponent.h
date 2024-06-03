// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/AnimAccessComponent.h"
#include "../../Data/FThrowableData.h"
#include "ThrowableComponent.generated.h"

class AEquipment;

USTRUCT()
struct FThrowable
{
	GENERATED_BODY()

public:
	FThrowable() {
		Amount = 0;
		Throwable = FThrowableDataItem();
	}

	FThrowable(FThrowableDataItem throwable, int amount) {
		Amount = amount;
		Throwable = throwable;
	}

	void Remove(int amount) {
		amount -= 1;
	}

	void Add(int amount) {
		Amount += amount;
	}

	FThrowableDataItem* GetThrowable() { return &Throwable; }
	int GetAmount() const { return Amount; }
protected:

	UPROPERTY() FThrowableDataItem Throwable;
	UPROPERTY() int Amount;

};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SV_API UThrowableComponent : public UAnimAccessComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UThrowableComponent(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	int GetThrowableAmount(EThrowable throwable);
	FThrowableDataItem* GetThrowableItem(EThrowable throwable);

	void AddThrowable(EThrowable throwable, int amount);

	void ThrowAtLocation(FVector location);

	void SpawnThrowableOfTypeAtRightHand(EThrowable throwable);
	AEquipment* GetThrownEquipment();

private:

	UPROPERTY() TArray<FThrowable> Throwables;
	UPROPERTY() FVector ThrowingDestination;

	UPROPERTY() AEquipment* ThrownEquipment;
	UPROPERTY() TArray<FVector> Trajectories;

};
