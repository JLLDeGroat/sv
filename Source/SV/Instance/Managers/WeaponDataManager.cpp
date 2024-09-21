// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponDataManager.h"

UWeaponDataManager::UWeaponDataManager() {
	ReadFile("Base/sv_wd", &WeaponData);
}

FWeaponData* UWeaponDataManager::GetWeaponData() {
	return &WeaponData;
}