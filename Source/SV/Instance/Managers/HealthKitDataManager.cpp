// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthKitDataManager.h"


UHealthKitDataManager::UHealthKitDataManager() {
	ReadFile("Base/sv_hkd", &HealthKitData);
}
FHealthKitData* UHealthKitDataManager::GetHealthKitData() {
	return &HealthKitData;
}