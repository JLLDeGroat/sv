// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseRunnable.h"
#include "../Enums/EWorldEnums.h"
#include "LevelGenerationRunnable.generated.h"

class UBaseLevelIndex;


UCLASS()
class SV_API ULevelGenerationRunnable : public UBaseRunnable
{
	GENERATED_BODY()

public:
	virtual void ActivateThread() override;
	ULevelGenerationRunnable* InsertVariables(ELevelGenType levelGen);

	void AddDebugActor(AActor* actor);
	UPROPERTY() TArray<AActor*> DebugActors;

	virtual void KillThread() override;
private:

	UPROPERTY() ELevelGenType GenType;
	UPROPERTY() UBaseLevelIndex* LevelIndex;

};
