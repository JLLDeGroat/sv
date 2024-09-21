// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowableDataManager.h"

UThrowableDataManager::UThrowableDataManager() {
	ReadFile("Base/sv_td", &ThrowableData);
}
FThrowableData* UThrowableDataManager::GetThrowableData() {
	return &ThrowableData;
}