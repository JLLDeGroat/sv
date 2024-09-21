// Fill out your copyright notice in the Description page of Project Settings.


#include "SupplyDataManager.h"

USupplyDataManager::USupplyDataManager() {
	ReadFile("Base/sv_ssd", &SupplyStockData);
}

FSupplyStockData* USupplyDataManager::GetSupplyStockData() {
	return &SupplyStockData;
}