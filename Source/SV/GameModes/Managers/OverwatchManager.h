// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../Enums/EConfigurationEnums.h"
#include "OverwatchManager.generated.h"

USTRUCT()
struct FOverwatchItem
{
	GENERATED_BODY()
public:
	FOverwatchItem() {
		Overwatcher = nullptr;
		Target = nullptr;
		ComponentSaw = nullptr;
		bInProgress = false;
		bIsComplete = false;
	}

	FOverwatchItem(AActor* overwatcher, AActor* target, UPrimitiveComponent* componentSaw) {
		Overwatcher = overwatcher;
		Target = target;
		ComponentSaw = componentSaw;
		bInProgress = false;
		bIsComplete = false;
	}

	void SetIsInProgress(bool value) { bInProgress = value; }
	bool GetIsInProgress() const { return bInProgress; }

	void SetIsInComplete(bool value) { bIsComplete = value; }
	bool GetIsInComplete() const { return bIsComplete; }

	AActor* GetOverwatcher() { return Overwatcher; }
	AActor* GetTarget() { return Target; }
	UPrimitiveComponent* GetComponentSaw() { return ComponentSaw; }



protected:


private:

	UPROPERTY() AActor* Overwatcher;
	UPROPERTY() AActor* Target;
	UPROPERTY() UPrimitiveComponent* ComponentSaw;

	UPROPERTY() bool bInProgress;
	UPROPERTY() bool bIsComplete;

};


UCLASS()
class SV_API UOverwatchManager : public UObject
{
	GENERATED_BODY()
public:
	UOverwatchManager(const FObjectInitializer& ObjectInitializer);

	void AddToOverwatchPotentials(AActor* overwatcher, AActor* target, UPrimitiveComponent* componentSaw);

	void OnOverwatchItemComplete();

	bool GetIsCurrentlyOverwatchAnimating();

protected:

	void AttemptToStartAnOverwatch();

	FTimerHandle DelayTimerHandle;
	UFUNCTION() void DelayTimerHandleCallback();

	UPROPERTY() bool bCurrentlyOverwatchAnimating;
	UPROPERTY() TArray<FOverwatchItem> OverwatchItems;
	FOverwatchItem* GetCurrentOverwatchItem();

	UPROPERTY() FVector OriginalLocation;
	UPROPERTY() FRotator OriginalRotation;
};
