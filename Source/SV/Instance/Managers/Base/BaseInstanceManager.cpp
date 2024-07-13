// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInstanceManager.h"


void UBaseInstanceManager::ReadModData(FString modName, FString& fileText) {
	FString file = FPaths::ProjectModsDir() + modName + ".json";
	UDebugMessages::LogDisplay(this, file);
	FString returnString;
	if (!FFileHelper::LoadFileToString(returnString, *file)) {
		UDebugMessages::LogError(this, "failed to load mod data " + modName);
		return;
	}
	fileText = returnString;
}