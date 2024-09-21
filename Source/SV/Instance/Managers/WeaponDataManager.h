// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseInstanceManager.h"
#include "../../Data/FWeaponData.h"
#include "WeaponDataManager.generated.h"

/**
 * 
 */
UCLASS()
class SV_API UWeaponDataManager : public UBaseInstanceManager
{
	GENERATED_BODY()
	
public:

	UWeaponDataManager();

	FWeaponData* GetWeaponData();

protected:
	UPROPERTY() FWeaponData WeaponData;
};
