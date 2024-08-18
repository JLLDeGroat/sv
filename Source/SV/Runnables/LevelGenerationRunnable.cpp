// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerationRunnable.h"
#include "VgCore/Domain/Debug/DebugMessages.h"
#include "Generations/LevelIndex/GenericLevel.h"
#include "Generations/LevelIndex/TwoBuildingsLevel.h"


#pragma optimize("", off)
ULevelGenerationRunnable* ULevelGenerationRunnable::InsertVariables(ELevelGenType levelGen) {
	GenType = levelGen;
	return this;
}

void ULevelGenerationRunnable::KillThread() {
	Super::KillThread();
	if (LevelIndex) {
		LevelIndex->KillObject();
	}
}

void ULevelGenerationRunnable::ActivateThread() {
	UDebugMessages::LogError(this, "DEBUG SET AND WILL NOT GENERATE LEVEL");
	bool bDebug = false;
	if (!bDebug)return;

	LevelIndex = nullptr;

	switch (GenType) {
	case ELevelGenType::TwoBuilding:
		LevelIndex = NewObject<UTwoBuildingsLevel>();
		break;
	case ELevelGenType::Generic:
		LevelIndex = NewObject<UGenericLevel>();
		break;

	default:
		UDebugMessages::LogError(this, "no gen class for this level index");
	}

	if (LevelIndex)
		LevelIndex
		->Setup(GetWorld(), RandomStream)
		->GenerateLevel();
}
#pragma optimize("", on)
