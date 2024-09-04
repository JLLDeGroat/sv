// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectivesManager.generated.h"


USTRUCT()
struct FObjectiveItem
{
	GENERATED_BODY()
public:
	FObjectiveItem() {
		bIsValid = false;
	}

	FObjectiveItem(FString objectiveText) {
		bIsValid = true;
		ObjectiveText = objectiveText;
	}

	FString GetObjectiveText() { return ObjectiveText; }
	void UpdateObjectiveText(FString txt) { ObjectiveText = txt; }

	void SetIsValid() { bIsValid = true; }
protected:

	UPROPERTY() FString ObjectiveText;

private:

	UPROPERTY() bool bIsValid;

};


/**
 *
 */
UCLASS()
class SV_API UObjectivesManager : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION() void SetupMainObjective();

	void UpdateMainObjective(FString obj = "");


private:

	UPROPERTY() FObjectiveItem MainObjective;
	UPROPERTY() FObjectiveItem SubObjectiveOne;
	UPROPERTY() FObjectiveItem SubObjectiveTwo;


	FTimerHandle MainDelayerHandle;
	UFUNCTION() void MainDelayerHandleCallback();

};
